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



#include "../header/the_lo3ba.h"

// Cast a single ray using DDA algorithm
void cast_single_ray(t_map *game, double ray_angle)
{
    // Start from player center
    double ray_x = game->player.player_x + PLAYER_OFFSET + PLAYER_SIZE/2;
    double ray_y = game->player.player_y + PLAYER_OFFSET + PLAYER_SIZE/2;
    
    double start_x = ray_x;
    double start_y = ray_y;
    
    double ray_dx = cos(DEG_TO_RAD(ray_angle));
    double ray_dy = sin(DEG_TO_RAD(ray_angle));
    
    // Current grid position
    int map_x = (int)(ray_x / TILE);
    int map_y = (int)(ray_y / TILE);
    
    // DDA setup
    double delta_dist_x = fabs(1.0 / ray_dx);
    double delta_dist_y = fabs(1.0 / ray_dy);
    
    double side_dist_x, side_dist_y;
    int step_x, step_y;
    
    // Calculate step direction and initial distances
    if (ray_dx < 0) {
        step_x = -1;
        side_dist_x = (ray_x / TILE - map_x) * delta_dist_x;
    } else {
        step_x = 1;
        side_dist_x = (map_x + 1.0 - ray_x / TILE) * delta_dist_x;
    }
    
    if (ray_dy < 0) {
        step_y = -1;
        side_dist_y = (ray_y / TILE - map_y) * delta_dist_y;
    } else {
        step_y = 1;
        side_dist_y = (map_y + 1.0 - ray_y / TILE) * delta_dist_y;
    }
    
    // DDA loop - move tile by tile
    int hit = 0;
    double wall_x = 0, wall_y = 0;
    
    while (!hit && map_x >= 0 && map_x < game->width && map_y >= 0 && map_y < game->height)
    {
        // Check horizontal intersection first
        if (side_dist_y < side_dist_x)
        {
            side_dist_y += delta_dist_y;
            map_y += step_y;
            
            // Check bounds and wall
            if (map_y >= 0 && map_y < game->height && map_x >= 0 && map_x < game->width)
            {
                if (game->map[map_y][map_x] == '1')
                {
                    double distance = (map_y - ray_y / TILE + (1 - step_y) / 2) / ray_dy * TILE;
                    wall_x = ray_x + distance * ray_dx;
                    wall_y = map_y * TILE + (step_y < 0 ? TILE : 0);
                    hit = 1;
                }
            }
            else
                break; // Out of bounds
        }
        // Then check vertical intersection
        else
        {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            
            // Check bounds and wall
            if (map_x >= 0 && map_x < game->width && map_y >= 0 && map_y < game->height)
            {
                if (game->map[map_y][map_x] == '1')
                {
                    double distance = (map_x - ray_x / TILE + (1 - step_x) / 2) / ray_dx * TILE;
                    wall_y = ray_y + distance * ray_dy;
                    wall_x = map_x * TILE + (step_x < 0 ? TILE : 0);
                    hit = 1;
                }
            }
            else
                break; // Out of bounds
        }
    }
    
    // Draw the ray if we hit something - using YOUR pixel_put_img function
    if (hit)
    {
        double dx = wall_x - start_x;
        double dy = wall_y - start_y;
        double steps = fmax(fabs(dx), fabs(dy));
        
        if (steps > 0)
        {
            double x_inc = dx / steps;
            double y_inc = dy / steps;
            
            // Draw ray line using YOUR function
            for (int i = 0; i <= (int)steps; i += 3) // Skip pixels for performance
            {
                int x = (int)(start_x + i * x_inc);
                int y = (int)(start_y + i * y_inc);
                
                // Use YOUR pixel_put_img function with bounds check
                if (x >= 0 && y >= 0 && x < game->width * TILE && y < game->height * TILE)
                    pixel_put_img(game, x, y, 0xFFFF00); // Yellow ray
            }
            
            // Draw wall hit point
            if ((int)wall_x >= 0 && (int)wall_y >= 0 && 
                (int)wall_x < game->width * TILE && (int)wall_y < game->height * TILE)
                pixel_put_img(game, (int)wall_x, (int)wall_y, 0xFF0000); // Red dot
        }
    }
}

// Cast all FOV rays
void cast_fov_rays(t_map *game)
{
    double fov = 60.0;
    double angle_step = fov / ray_num;
    double start_angle = game->player.angle - (fov / 2.0);
    
    for (int i = 0; i < ray_num; i++)
    {
        double current_angle = start_angle + (i * angle_step);
        
        // Normalize angle
        while (current_angle < 0) current_angle += 360;
        while (current_angle >= 360) current_angle -= 360;
        
        // Cast ray using DDA
        cast_single_ray(game, current_angle);
    }
}