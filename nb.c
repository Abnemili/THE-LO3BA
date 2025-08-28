#include "../header/the_lo3ba.h"

typedef struct s_player{
    double player_x;            
    double player_y;
    double angle;           
}t_player;

typedef struct s_map {
    void    *mlx;
    void    *win;
    void    *img;           
    char    *img_data;      
    int     img_bpp;
    int     img_size_line;
    int     img_endian;
    char    **map;
    int     width;
    int     height;
    t_player player;
}t_map;

// MODIFIED: Added hit info structure for storing ray results
typedef struct s_hit {
    double distance;
    double hit_x;
    double hit_y;
    int is_vertical;  // 0 = horizontal wall, 1 = vertical wall
} t_hit;

#define TILE 32
#define M_PI 3.14159265358979323846
#define DEG_TO_RAD(angleDegrees) ((angleDegrees) * M_PI / 180.0)

#define PLAYER_COLOR 0x00FF00    
#define COLOR_WALL   0X8B0000    
#define COLOR_FREE   0x000000    

#define MOVE_SPEED 8  
#define PLAYER_SIZE 8 
#define PLAYER_OFFSET 12 

#define KEY_ESC 65307
#define KEY_W   119
#define KEY_S   115
#define KEY_A   97
#define KEY_D   100
#define ray_num 32

void draw_pixel(t_map *game, int x, int y, int color)
{
    if (x < 0 || y < 0 || x >= game->width * TILE || y >= game->height * TILE)
        return;

    int index = (y * game->img_size_line) + (x * (game->img_bpp / 8));
    unsigned int *pixel = (unsigned int *)(game->img_data + index);
    *pixel = color;
}

// MODIFIED: Check horizontal line intersections first
t_hit check_horizontal_hit(t_map *game, double ray_angle)
{
    t_hit hit = {0};
    double px = game->player.player_x + PLAYER_OFFSET + PLAYER_SIZE/2;
    double py = game->player.player_y + PLAYER_OFFSET + PLAYER_SIZE/2;
    
    double ray_angle_rad = DEG_TO_RAD(ray_angle);
    
    // Avoid division by zero
    if (sin(ray_angle_rad) == 0)
    {
        hit.distance = 999999;
        return hit;
    }
    
    int looking_up = sin(ray_angle_rad) < 0;
    
    // Find first horizontal grid line
    double y_step = TILE;
    double first_y = floor(py / TILE) * TILE;
    if (!looking_up)
        first_y += TILE;
    else
        y_step = -TILE;
    
    // Calculate x step based on angle
    double x_step = TILE / tan(ray_angle_rad);
    if ((looking_up && x_step > 0) || (!looking_up && x_step < 0))
        x_step = -x_step;
    
    double check_x = px + (first_y - py) / tan(ray_angle_rad);
    double check_y = first_y;
    
    // Step through horizontal lines
    while (check_x >= 0 && check_x < game->width * TILE && 
           check_y >= 0 && check_y < game->height * TILE)
    {
        int map_x = (int)(check_x / TILE);
        int map_y = (int)(check_y / TILE);
        
        // Adjust for looking up
        if (looking_up)
            map_y--;
            
        if (map_x < 0 || map_y < 0 || map_x >= game->width || map_y >= game->height)
            break;
            
        if (game->map[map_y][map_x] == '1')
        {
            hit.hit_x = check_x;
            hit.hit_y = check_y;
            hit.distance = sqrt((check_x - px) * (check_x - px) + (check_y - py) * (check_y - py));
            hit.is_vertical = 0;
            return hit;
        }
        
        check_x += x_step;
        check_y += y_step;
    }
    
    hit.distance = 999999;
    return hit;
}

// MODIFIED: Check vertical line intersections
t_hit check_vertical_hit(t_map *game, double ray_angle)
{
    t_hit hit = {0};
    double px = game->player.player_x + PLAYER_OFFSET + PLAYER_SIZE/2;
    double py = game->player.player_y + PLAYER_OFFSET + PLAYER_SIZE/2;
    
    double ray_angle_rad = DEG_TO_RAD(ray_angle);
    
    // Avoid division by zero
    if (cos(ray_angle_rad) == 0)
    {
        hit.distance = 999999;
        return hit;
    }
    
    int looking_left = cos(ray_angle_rad) < 0;
    
    // Find first vertical grid line
    double x_step = TILE;
    double first_x = floor(px / TILE) * TILE;
    if (!looking_left)
        first_x += TILE;
    else
        x_step = -TILE;
    
    // Calculate y step based on angle
    double y_step = TILE * tan(ray_angle_rad);
    if ((looking_left && y_step < 0) || (!looking_left && y_step > 0))
        y_step = -y_step;
    
    double check_x = first_x;
    double check_y = py + (first_x - px) * tan(ray_angle_rad);
    
    // Step through vertical lines
    while (check_x >= 0 && check_x < game->width * TILE && 
           check_y >= 0 && check_y < game->height * TILE)
    {
        int map_x = (int)(check_x / TILE);
        int map_y = (int)(check_y / TILE);
        
        // Adjust for looking left
        if (looking_left)
            map_x--;
            
        if (map_x < 0 || map_y < 0 || map_x >= game->width || map_y >= game->height)
            break;
            
        if (game->map[map_y][map_x] == '1')
        {
            hit.hit_x = check_x;
            hit.hit_y = check_y;
            hit.distance = sqrt((check_x - px) * (check_x - px) + (check_y - py) * (check_y - py));
            hit.is_vertical = 1;
            return hit;
        }
        
        check_x += x_step;
        check_y += y_step;
    }
    
    hit.distance = 999999;
    return hit;
}

// MODIFIED: Cast single ray using DDA - check horizontal first, then vertical
t_hit cast_single_ray(t_map *game, double ray_angle)
{
    // Check horizontal intersections first
    t_hit h_hit = check_horizontal_hit(game, ray_angle);
    
    // Check vertical intersections
    t_hit v_hit = check_vertical_hit(game, ray_angle);
    
    // Return the closest hit
    if (h_hit.distance < v_hit.distance)
        return h_hit;
    else
        return v_hit;
}

// MODIFIED: Main function to cast all rays
void cast_all_rays(t_map *game)
{
    double fov = 60.0;                          
    int window_width = game->width * TILE;      
    int num_rays = window_width;                
   
    double angle_step = fov / (double)num_rays; 
    double start_angle = game->player.angle - (fov / 2.0); 
    
    for (int i = 0; i < num_rays; i++)
    {
        double current_angle = start_angle + (i * angle_step);
        
        // Normalize angle
        while (current_angle < 0) 
            current_angle += 360.0;
        while (current_angle >= 360.0) 
            current_angle -= 360.0;
        
        // Cast ray and get hit info
        t_hit hit = cast_single_ray(game, current_angle);
        
        // Draw ray line for visualization (optional)
        if (hit.distance < 999999)
        {
            double px = game->player.player_x + PLAYER_OFFSET + PLAYER_SIZE/2;
            double py = game->player.player_y + PLAYER_OFFSET + PLAYER_SIZE/2;
            
            double dx = hit.hit_x - px;
            double dy = hit.hit_y - py;
            double steps = hit.distance;
            
            for (int j = 0; j < (int)steps; j++)
            {
                int draw_x = (int)(px + (dx * j / steps));
                int draw_y = (int)(py + (dy * j / steps));
                
                // Different colors for horizontal vs vertical hits
                int color = hit.is_vertical ? 0xFF0000 : 0x00FF00; // Red for vertical, Green for horizontal
                draw_pixel(game, draw_x, draw_y, color);
            }
        }
        
        // Here you can store hit.distance, hit.hit_x, hit.hit_y for 3D rendering
        // For example: game->ray_distances[i] = hit.distance;
    }
}

// MODIFIED: Sparse ray casting for better visualization
void cast_fov_rays_sparse(t_map *game, int ray_spacing)
{
    double fov = 60.0;                          
    int window_width = game->width * TILE;      
    int num_rays = window_width / ray_spacing;  
    
    double angle_step = fov / (double)num_rays;
    double start_angle = game->player.angle - (fov / 2.0);
    
    for (int i = 0; i < num_rays; i++)
    {
        double current_angle = start_angle + (i * angle_step);
        
        // Normalize angle
        while (current_angle < 0) 
            current_angle += 360.0;
        while (current_angle >= 360.0) 
            current_angle -= 360.0;
        
        // Cast ray and get hit info
        t_hit hit = cast_single_ray(game, current_angle);
        
        // Draw ray for visualization
        if (hit.distance < 999999)
        {
            double px = game->player.player_x + PLAYER_OFFSET + PLAYER_SIZE/2;
            double py = game->player.player_y + PLAYER_OFFSET + PLAYER_SIZE/2;
            
            double dx = hit.hit_x - px;
            double dy = hit.hit_y - py;
            double steps = hit.distance;
            
            for (int j = 0; j < (int)steps; j++)
            {
                int draw_x = (int)(px + (dx * j / steps));
                int draw_y = (int)(py + (dy * j / steps));
                
                int color = hit.is_vertical ? 0xFF0000 : 0x00FF00;
                draw_pixel(game, draw_x, draw_y, color);
            }
        }
    }
}

int handle_key_input(int keycode, t_map *map)
{
    int new_x, new_y;
    
    if (keycode == KEY_ESC)
        return (handle_close(map));
    
    if (keycode == 65361) // Left arrow
    {
        map->player.angle -= 5.0;
        if (map->player.angle < 0)
            map->player.angle += 360;
        
        set_color(map);
        draw_player(map);
        cast_fov_rays_sparse(map, ray_num);
        return (0);
    }
    else if (keycode == 65363) // Right arrow
    {
        map->player.angle += 5.0;
        if (map->player.angle >= 360)
            map->player.angle -= 360;
        
        set_color(map);
        draw_player(map);
        cast_fov_rays_sparse(map, ray_num);
        return (0);
    }
    
    new_x = map->player.player_x;
    new_y = map->player.player_y;
    
    if (keycode == KEY_W)
        new_y -= MOVE_SPEED;
    else if (keycode == KEY_S)
        new_y += MOVE_SPEED;
    else if (keycode == KEY_A)
        new_x -= MOVE_SPEED;
    else if (keycode == KEY_D)
        new_x += MOVE_SPEED;
    else
        return (0);
    
    if (is_valid_move(map, new_x, new_y))
    {
        set_color(map);
        map->player.player_x = new_x;
        map->player.player_y = new_y;
        draw_player(map);
        cast_fov_rays_sparse(map, ray_num);
    }
    
    return (0);
}