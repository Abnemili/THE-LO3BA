#include "../header/the_lo3ba.h"

typedef struct s_player{
    double player_x;            // x and y are the player position cordinate 
    double player_y;
    double angle;           //player view direction in degree
}t_player;


typedef struct s_map {
    void    *mlx;
    void    *win;
    void    *img;           // Image buffer
    char    *img_data;      // Image data pointer
    int     img_bpp;
    int     img_size_line;
    int     img_endian;
    char    **map;
    int     width;
    int     height;
    t_player player;
}t_map;


/* Game settings */
#define TILE 32
#define M_PI 3.14159265358979323846
#define DEG_TO_RAD(angleDegrees) ((angleDegrees) * M_PI / 180.0)


/* Colors */
#define PLAYER_COLOR 0x00FF00    /* Green */
#define COLOR_WALL   0X8B0000    /* White */
#define COLOR_FREE   0x000000    /* Black */

#define MOVE_SPEED 8  // Pixels per keypress
#define PLAYER_SIZE 8 // Size of player square
#define PLAYER_OFFSET 12 // Offset from tile corner

/* Key codes */
#define KEY_ESC 65307
#define KEY_W   119
#define KEY_S   115
#define KEY_A   97
#define KEY_D   100
#define ray_num 32


// Draw a pixel at (x, y) with the given color on the game image
void draw_pixel(t_map *game, int x, int y, int color)
{
    // Check if the coordinates are within the drawable area to avoid out-of-bounds access
    if (x < 0 || y < 0 || x >= game->width * TILE
            || y >= game->height * TILE)
        return;


    int index = (y * game->img_size_line) + (x * (game->img_bpp / 8));

    // Get a pointer to the pixel's memory location inside the image buffer
    unsigned int *pixel = (unsigned int *)(game->img_data + index);

    // Set the pixel color at this location
    *pixel = color;
}


// Cast a single ray at a specific angle and draw it
void cast_single_ray(t_map *game, double ray_angle)
{
    double ray_x = game->player.player_x + PLAYER_OFFSET + PLAYER_SIZE/2;
    double ray_y = game->player.player_y + PLAYER_OFFSET + PLAYER_SIZE/2;

    double ray_angle_rad = DEG_TO_RAD(ray_angle);
    double ray_dx = cos(ray_angle_rad);
    double ray_dy = sin(ray_angle_rad);

    double step_size = 0.5;
    int max_depth = 500;
    int i = 0;

    while (i < max_depth)
    {
        int map_x = (int)(ray_x / TILE);
        int map_y = (int)(ray_y / TILE);

        if (map_x < 0 || map_y < 0 || map_y >= game->height || map_x >= game->width)
            break;

        if (game->map[map_y][map_x] == '1')
            break;

        int pixel_x = (int)ray_x;
        int pixel_y = (int)ray_y;
        if (pixel_x >= 0 && pixel_y >= 0 && 
            pixel_x < game->width * TILE && pixel_y < game->height * TILE)
        {
            mlx_pixel_put(game->mlx, game->win, pixel_x, pixel_y, 0xFFFF00);
        }

        ray_x += ray_dx * step_size;
        ray_y += ray_dy * step_size;
        i++;
    }
}

// Cast all rays for 60-degree FOV based on window width
void cast_fov_rays(t_map *game)
{
    double fov = 60.0;                          // Field of view in degrees (static)
    int window_width = game->width * TILE;      // Total window width in pixels
    int num_rays = window_width;                // One ray per pixel column
   
    
    double angle_step = fov / (double)num_rays; // Angle between each ray
    double start_angle = game->player.angle - (fov / 2.0); // Left edge of FOV
    
    for (int i = 0; i < num_rays; i++)
    {
        double current_angle = start_angle + (i * angle_step);
        
        // Normalize angle to [0, 360) range
        while (current_angle < 0) 
            current_angle += 360.0;
        while (current_angle >= 360.0) 
            current_angle -= 360.0;
        
        // Cast this ray
        cast_single_ray(game, current_angle);
    }
    
    printf("Finished casting all rays\n");
}

// Alternative: Cast fewer rays for better visualization (every Nth column)
void cast_fov_rays_sparse(t_map *game, int ray_spacing)
{
    double fov = 60.0;                          // Field of view in degrees
    int window_width = game->width * TILE;      // Total window width in pixels
    int num_rays = window_width / ray_spacing;  // Fewer rays for visualization
    
    
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
        
        cast_single_ray(game, current_angle);
    }
}
int handle_key_input(int keycode, t_map *map)
{
    int new_x, new_y;
    
    if (keycode == KEY_ESC)// the escape kety was pressed 
        return (handle_close(map));
    
    // Handle rotation first
    if (keycode == 65361) // Left arrow key
    {
        map->player.angle -= 5.0;
        if (map->player.angle < 0)
            map->player.angle += 360;
        
        // Clear and redraw for rotation
        // mlx_clear_window(map->mlx, map->win);
        set_color(map);
        draw_player(map);
        // draw_ray(map);  // Add this line
        cast_fov_rays_sparse(map, ray_num);
        // cast_fov_rays(map);
        return (0);
    }
    else if (keycode == 65363) // Right arrow key
    {
        map->player.angle += 5.0;
        if (map->player.angle >= 360)
            map->player.angle -= 360;
        
        // Clear and redraw for rotation  
        // mlx_clear_window(map->mlx, map->win);
        set_color(map);
        draw_player(map);
        // draw_ray(map);  // Add this line
        cast_fov_rays_sparse(map, ray_num);
        // cast_fov_rays(map);
        return (0);
    }                                                                                       
    
    // Calculate new position for movement
    new_x = map->player.player_x;
    new_y = map->player.player_y;
    
    if (keycode == KEY_W) // W - Up
        new_y -= MOVE_SPEED;
    else if (keycode == KEY_S) // S - Down
        new_y += MOVE_SPEED;
    else if (keycode == KEY_A) // A - Left
        new_x -= MOVE_SPEED;
    else if (keycode == KEY_D) // D - Right
        new_x += MOVE_SPEED;
    else
        return (0);
    
    // Check if move is valid
    if (is_valid_move(map, new_x, new_y))
    {
        // Clear and redraw for movement
        // mlx_clear_window(map->mlx, map->win);
        set_color(map);
        
        // Update position
        map->player.player_x = new_x;
        map->player.player_y = new_y;
        
        // Draw player and ray
        draw_player(map);
        // draw_ray(map);  // This should already be here
        cast_fov_rays_sparse(map, ray_num);
        // cast_fov_rays(map);

    }
    
    return (0);
}
