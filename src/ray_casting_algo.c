#include "../header/the_lo3ba.h"

// Cast a single ray using DDA algorithm
void cast_single_ray(t_map *game, double ray_angle)
{
    double ray_x = game->player.player_x + PLAYER_OFFSET + PLAYER_SIZE/2;
    double ray_y = game->player.player_y + PLAYER_OFFSET + PLAYER_SIZE/2;
    
    double start_x = ray_x;
    double start_y = ray_y;
    
    double ray_dx = cos(DEG_TO_RAD(ray_angle));
    double ray_dy = sin(DEG_TO_RAD(ray_angle));
    
    int map_x = (int)(ray_x / TILE);
    int map_y = (int)(ray_y / TILE);
    
    double delta_dist_x = fabs(1.0 / ray_dx);
    double delta_dist_y = fabs(1.0 / ray_dy);
    
    double side_dist_x, side_dist_y;
    int step_x, step_y;
    
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
    
    int hit = 0;
    double wall_x = 0, wall_y = 0;
    
    while (!hit && map_x >= 0 && map_x < game->width && map_y >= 0 && map_y < game->height)
    {
        // Check horizontal intersection first
        if (side_dist_y < side_dist_x)
        {
            side_dist_y += delta_dist_y;
            map_y += step_y;
            
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
                break;
        }
        // Then check vertical intersection
        else
        {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            
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
                break;
        }
    }
    
    // Draw ray to image buffer
    if (hit)
    {
        double dx = wall_x - start_x;
        double dy = wall_y - start_y;
        double steps = fmax(fabs(dx), fabs(dy));
        
        if (steps > 0)
        {
            double x_inc = dx / steps;
            double y_inc = dy / steps;
            
            for (int i = 0; i <= (int)steps; i += 2)
            {
                int x = (int)(start_x + i * x_inc);
                int y = (int)(start_y + i * y_inc);
                pixel_put_img(game, x, y, 0xFFFF00); // Yellow ray
            }
            
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
        
        while (current_angle < 0) current_angle += 360;
        while (current_angle >= 360) current_angle -= 360;
        
        cast_single_ray(game, current_angle);
    }
}