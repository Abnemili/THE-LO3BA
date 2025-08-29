#include "../header/the_lo3ba.h"

// // Draw a pixel at (x, y) with the given color on the game image
// void draw_pixel(t_map *game, int x, int y, int color)
// {

//     if (x < 0 || y < 0 || x >= game->width * TILE
//             || y >= game->height * TILE)
//         return;
//     int index = (y * game->img_size_line) + (x * (game->img_bpp / 8));

//     unsigned int *pixel = (unsigned int *)(game->img_data + index);


//     *pixel = color;
// }

// // Cast a single ray at a specific angle and draw it
// void cast_single_ray(t_map *game, double ray_angle)
// {
//     double ray_x = game->player.player_x + PLAYER_OFFSET + PLAYER_SIZE/2;
//     double ray_y = game->player.player_y + PLAYER_OFFSET + PLAYER_SIZE/2;

//     double ray_angle_rad = DEG_TO_RAD(ray_angle);
//     double ray_dx = cos(ray_angle_rad);
//     double ray_dy = sin(ray_angle_rad);

//     double step_size = 0.7;
//     int max_depth = 500;
//     int i = 0;

//     while (i < max_depth)
//     {
//         int map_x = (int)(ray_x / TILE);
//         int map_y = (int)(ray_y / TILE);

//         if (map_x < 0 || map_y < 0 || map_y >= game->height || map_x >= game->width)
//             break;

//         if (game->map[map_y][map_x] == '1')
//             break;

//         int pixel_x = (int)ray_x;
//         int pixel_y = (int)ray_y;
//         if (pixel_x >= 0 && pixel_y >= 0 && 
//             pixel_x < game->width * TILE && pixel_y < game->height * TILE)
//         {
//             mlx_pixel_put(game->mlx, game->win, pixel_x, pixel_y, 0xFFFF00);
//         }

//         ray_x += ray_dx * step_size;
//         ray_y += ray_dy * step_size;
//         i++;
//     }
// }
// // Alternative: Cast fewer rays for better visualization (every Nth column)
// void cast_fov_rays_sparse(t_map *game, int ray_spacing)
// {
//     double fov = 60.0;                          // Field of view in degrees
//     int window_width = game->width * TILE;      // Total window width in pixels
//     int num_rays = window_width / ray_spacing;  // Fewer rays for visualization
    
    
//     double angle_step = fov / (double)num_rays;
//     double start_angle = game->player.angle - (fov / 2.0);
    
//     for (int i = 0; i < num_rays; i++)
//     {
//         double current_angle = start_angle + (i * angle_step);
        
//         // Normalize angle
//         while (current_angle < 0) 
//             current_angle += 360.0;
//         while (current_angle >= 360.0) 
//             current_angle -= 360.0;
        
//         cast_single_ray(game, current_angle);
//             printf("Finished casting all rays\n");
//     }
// }


// Check if a grid position contains a wall
int is_wall(t_map *map, int grid_x, int grid_y)
{
    if (grid_x < 0 || grid_x >= map->width || grid_y < 0 || grid_y >= map->height)
        return (1); // Out of bounds = wall
    return (map->map[grid_y][grid_x] == '1');
}

// Cast a single ray using DDA algorithm
static t_ray cast_single_ray(t_map *map, double ray_angle)
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

// Cast FOV rays using DDA algorithm
void cast_fov_rays(t_map *map)
{
    double fov = 60.0; // Field of view in degrees
    double angle_step = fov / ray_num;
    double start_angle = map->player.angle - (fov / 2.0);
    
    printf("=== Casting %d FOV rays ===\n", ray_num);
    printf("Player position: (%.2f, %.2f)\n", map->player.player_x, map->player.player_y);
    printf("Player angle: %.2f degrees\n", map->player.angle);
    printf("FOV: %.2f degrees (%.2f to %.2f)\n", fov, start_angle, start_angle + fov);
    
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
        
        printf("Ray %2d: angle=%.2f°, wall=(%.2f,%.2f), dist=%.2f, side=%s\n",
               i, current_angle, ray.wall_x, ray.wall_y, ray.distance,
               ray.hit_side == 0 ? "vertical" : "horizontal");
        
        // Here you can store the ray data or use it for rendering
        // For example, you might store it in an array for later use:
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