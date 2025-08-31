#include <math.h>
#include <stdio.h>
#include "header/the_lo3ba.h"


typedef struct s_ray {
    double wall_x;      // Wall hit x coordinate
    double wall_y;      // Wall hit y coordinate
    double distance;    // Distance to wall
    int hit_side;       // 0 if horizontal wall, 1 if vertical wall
} t_ray;

// Check if a grid position contains a wall
int is_wall(t_map *map, int grid_x, int grid_y)
{
    if (grid_x < 0 || grid_x >= map->width || grid_y < 0 || grid_y >= map->height)
        return (1); // Out of bounds = wall
    return (map->map[grid_y][grid_x] == '1');
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
    
    // Perform DDA
    while (hit == 0)
    {
        // Jump to next map square, either in x-direction, or in y-direction
        if (side_dist_x < side_dist_y)
        {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            side = 0; // Vertical wall (hit from x-direction)
        }
        else
        {
            side_dist_y += delta_dist_y;
            map_y += step_y;
            side = 1; // Horizontal wall (hit from y-direction)
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

// Draw a line using Bresenham's algorithm (for ray visualization)
void draw_line(t_map *map, int x0, int y0, int x1, int y1, int color)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    
    int x = x0;
    int y = y0;
    
    while (1)
    {
        // Check bounds before drawing
        if (x >= 0 && x < map->width * TILE && y >= 0 && y < map->height * TILE)
        {
            int pixel_index = (y * map->img_size_line) + (x * (map->img_bpp / 8));
            *(int *)(map->img_data + pixel_index) = color;
        }
        
        if (x == x1 && y == y1)
            break;
            
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y += sy;
        }
    }
}

// Draw a single ray from player to wall hit point
void draw_ray(t_map *map, t_ray ray)
{
    int start_x = (int)map->player.player_x;
    int start_y = (int)map->player.player_y;
    int end_x = (int)ray.wall_x;
    int end_y = (int)ray.wall_y;
    
    // Use different colors for horizontal vs vertical walls
    int ray_color = (ray.hit_side == 0) ? 0xFF0000 : 0x0000FF; // Red for vertical, Blue for horizontal
    
    draw_line(map, start_x, start_y, end_x, end_y, ray_color);
}

// Cast and draw FOV rays using DDA algorithm
void cast_fov_rays(t_map *map)
{
    double fov = 60.0; // Field of view in degrees
    double angle_step = fov / ray_num;
    double start_angle = map->player.angle - (fov / 2.0);
    
    for (int i = 0; i < ray_num; i++)
    {
        double current_angle = start_angle + (i * angle_step);
        
        // Normalize angle to 0-360 range
        while (current_angle < 0)
            current_angle += 360;
        while (current_angle >= 360)
            current_angle -= 360;
        
        // Cast single ray
        t_ray ray = cast_single_ray(map, current_angle);
        
        // Draw the ray
        draw_ray(map, ray);
        
        // Optional: Store ray data for later use
        // map->rays[i] = ray;
    }
}

// Alternative implementation with separate horizontal and vertical checks
// (if you specifically want to check horizontal intersections first)
t_ray cast_single_ray_separate_checks(t_map *map, double ray_angle)
{
    t_ray horizontal_ray = {0, 0, INFINITY, 1};
    t_ray vertical_ray = {0, 0, INFINITY, 0};
    
    double ray_x = map->player.player_x;
    double ray_y = map->player.player_y;
    double ray_dir_x = cos(DEG_TO_RAD(ray_angle));
    double ray_dir_y = sin(DEG_TO_RAD(ray_angle));
    
    // === Check Horizontal Intersections First ===
    if (ray_dir_y != 0) // Avoid division by zero
    {
        int step_y = (ray_dir_y > 0) ? 1 : -1;
        int grid_y = (int)(ray_y / TILE) + (step_y > 0 ? 1 : 0);
        
        while (grid_y >= 0 && grid_y < map->height)
        {
            double intersect_y = grid_y * TILE;
            double intersect_x = ray_x + (intersect_y - ray_y) * ray_dir_x / ray_dir_y;
            
            int grid_x = (int)(intersect_x / TILE);
            
            if (is_wall(map, grid_x, (step_y > 0) ? grid_y : grid_y - 1))
            {
                horizontal_ray.wall_x = intersect_x;
                horizontal_ray.wall_y = intersect_y;
                horizontal_ray.distance = sqrt((intersect_x - ray_x) * (intersect_x - ray_x) + 
                                             (intersect_y - ray_y) * (intersect_y - ray_y));
                break;
            }
            grid_y += step_y;
        }
    }
    
    // === Check Vertical Intersections ===
    if (ray_dir_x != 0) // Avoid division by zero
    {
        int step_x = (ray_dir_x > 0) ? 1 : -1;
        int grid_x = (int)(ray_x / TILE) + (step_x > 0 ? 1 : 0);
        
        while (grid_x >= 0 && grid_x < map->width)
        {
            double intersect_x = grid_x * TILE;
            double intersect_y = ray_y + (intersect_x - ray_x) * ray_dir_y / ray_dir_x;
            
            int grid_y = (int)(intersect_y / TILE);
            
            if (is_wall(map, (step_x > 0) ? grid_x : grid_x - 1, grid_y))
            {
                vertical_ray.wall_x = intersect_x;
                vertical_ray.wall_y = intersect_y;
                vertical_ray.distance = sqrt((intersect_x - ray_x) * (intersect_x - ray_x) + 
                                           (intersect_y - ray_y) * (intersect_y - ray_y));
                break;
            }
            grid_x += step_x;
        }
    }
    
    // Return the closest intersection
    return (horizontal_ray.distance < vertical_ray.distance) ? horizontal_ray : vertical_ray;
}