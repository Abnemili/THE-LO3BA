#include <math.h>
#include <stdio.h>
#include "header/the_lo3ba.h"

typedef struct s_ray {
    double wall_x;      // Wall hit x coordinate
    double wall_y;      // Wall hit y coordinate
    double distance;    // Distance to wall
    int hit_side;       // 0 if vertical wall, 1 if horizontal wall
} t_ray;

// Check if a grid position contains a wall
int is_wall(t_map *map, int grid_x, int grid_y)
{
    if (grid_x < 0 || grid_x >= map->width || grid_y < 0 || grid_y >= map->height)
        return (1); // Out of bounds = wall
    return (map->map[grid_y][grid_x] == '1');
}

// Simple drawing function like yours
void draw_pixel(t_map *game, int x, int y, int color)
{
    if (x < 0 || y < 0 || x >= game->width * TILE || y >= game->height * TILE)
        return;
    
    int index = (y * game->img_size_line) + (x * (game->img_bpp / 8));
    unsigned int *pixel = (unsigned int *)(game->img_data + index);
    *pixel = color;
}

// Cast a single ray using DDA algorithm
t_ray cast_single_ray(t_map *map, double ray_angle)
{
    t_ray ray = {0, 0, 0, 0};
    
    // Ray starting position (player position)
    double ray_x = map->player.player_x;
    double ray_y = map->player.player_y;
    
    // Ray direction
    double ray_dir_x = cos(DEG_TO_RAD(ray_angle));
    double ray_dir_y = sin(DEG_TO_RAD(ray_angle));
    
    // Which grid cell we're in
    int map_x = (int)(ray_x / TILE);
    int map_y = (int)(ray_y / TILE);
    
    // Length of ray from current position to next x or y side
    double side_dist_x, side_dist_y;
    
    // Length of ray from one x-side to next x-side, or from one y-side to next y-side
    double delta_dist_x = fabs(1 / ray_dir_x);
    double delta_dist_y = fabs(1 / ray_dir_y);
    
    // What direction to step in x or y (either +1 or -1)
    int step_x, step_y;
    
    int hit = 0; // Was there a wall hit?
    int side;    // Was it a horizontal or vertical wall hit?
    
    // Calculate step direction and initial side_dist
    if (ray_dir_x < 0)
    {
        step_x = -1;
        side_dist_x = (ray_x / TILE - map_x) * delta_dist_x;
    }
    else
    {
        step_x = 1;
        side_dist_x = (map_x + 1.0 - ray_x / TILE) * delta_dist_x;
    }
    
    if (ray_dir_y < 0)
    {
        step_y = -1;
        side_dist_y = (ray_y / TILE - map_y) * delta_dist_y;
    }
    else
    {
        step_y = 1;
        side_dist_y = (map_y + 1.0 - ray_y / TILE) * delta_dist_y;
    }
    
    // Perform DDA - check horizontal intersections first, then vertical
    while (hit == 0)
    {
        // Check horizontal intersections first
        if (side_dist_y < side_dist_x)
        {
            side_dist_y += delta_dist_y;
            map_y += step_y;
            side = 1; // Horizontal wall
        }
        else // Then check vertical intersections
        {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            side = 0; // Vertical wall
        }
        
        // Check if ray has hit a wall
        if (is_wall(map, map_x, map_y))
            hit = 1;
    }
    
    // Calculate distance and wall hit coordinates
    if (side == 0) // Vertical wall
    {
        ray.distance = (map_x - ray_x / TILE + (1 - step_x) / 2) / ray_dir_x * TILE;
        ray.wall_x = map_x * TILE + (step_x < 0 ? TILE : 0);
        ray.wall_y = ray_y + ray.distance * ray_dir_y;
    }
    else // Horizontal wall
    {
        ray.distance = (map_y - ray_y / TILE + (1 - step_y) / 2) / ray_dir_y * TILE;
        ray.wall_y = map_y * TILE + (step_y < 0 ? TILE : 0);
        ray.wall_x = ray_x + ray.distance * ray_dir_x;
    }
    
    ray.hit_side = side;
    return (ray);
}

// Simple ray drawing using your stepping method
void draw_ray_simple(t_map *game, double ray_angle)
{
    double ray_x = game->player.player_x + PLAYER_OFFSET + PLAYER_SIZE/2;
    double ray_y = game->player.player_y + PLAYER_OFFSET + PLAYER_SIZE/2;

    double ray_angle_rad = DEG_TO_RAD(ray_angle);
    double ray_dx = cos(ray_angle_rad);
    double ray_dy = sin(ray_angle_rad);

    double step_size = 0.7;
    int max_depth = 500;
    int i = 0;

    while (i < max_depth)
    {
        int map_x = (int)(ray_x / TILE);
        int map_y = (int)(ray_y / TILE);

        // Bounds check
        if (map_x < 0 || map_y < 0 || map_y >= game->height || map_x >= game->width)
            break;

        // Wall check
        if (game->map[map_y][map_x] == '1')
            break;

        // Draw ray pixel
        int pixel_x = (int)ray_x;
        int pixel_y = (int)ray_y;
        draw_pixel(game, pixel_x, pixel_y, 0xFFFF00); // Yellow ray

        // Step forward
        ray_x += ray_dx * step_size;
        ray_y += ray_dy * step_size;
        i++;
    }
}

// Cast FOV rays using DDA algorithm but draw them simply
void cast_fov_rays(t_map *game)
{
    double fov = 60.0;  // Field of view in degrees
    double angle_step = fov / ray_num;
    double start_angle = game->player.angle - (fov / 2.0);
    
    for (int i = 0; i < ray_num; i++)
    {
        double current_angle = start_angle + (i * angle_step);
        
        // Normalize angle to 0-360 range
        while (current_angle < 0)
            current_angle += 360;
        while (current_angle >= 360)
            current_angle -= 360;
        
        // Get wall info using DDA
        t_ray ray_data = cast_single_ray(game, current_angle);
        
        // Draw ray using simple stepping method
        draw_ray_simple(game, current_angle);
        
        // Optional: Use ray_data for distance calculations, 3D rendering, etc.
        // printf("Ray %d: dist=%.1f, wall=(%.1f,%.1f)\n", 
        //        i, ray_data.distance, ray_data.wall_x, ray_data.wall_y);
    }
}