#include "../header/the_lo3ba.h"
x
// Draw a pixel at (x, y) with the given color on the game image
void draw_pixel(t_map *game, int x, int y, int color)
{
    if (x < 0 || y < 0 || x >= game->width * TILE || y >= game->height * TILE)
        return;

    int index = (y * game->img_size_line) + (x * (game->img_bpp / 8));
    unsigned int *pixel = (unsigned int *)(game->img_data + index);
    *pixel = color;
}

// DDA Algorithm - Cast a single ray and return distance to wall
double cast_single_ray_dda(t_map *game, double ray_angle)
{
    // Starting position (convert from pixel coordinates to map coordinates)
    double ray_x = (game->player.player_x + PLAYER_OFFSET + PLAYER_SIZE/2) / (double)TILE;
    double ray_y = (game->player.player_y + PLAYER_OFFSET + PLAYER_SIZE/2) / (double)TILE;

    // Ray direction
    double ray_angle_rad = DEG_TO_RAD(ray_angle);
    double ray_dx = cos(ray_angle_rad);
    double ray_dy = sin(ray_angle_rad);

    // Current map position
    int map_x = (int)ray_x;
    int map_y = (int)ray_y;

    // Length of ray from current position to x-side or y-side
    double delta_dist_x = (ray_dx == 0) ? 1e30 : fabs(1.0 / ray_dx);
    double delta_dist_y = (ray_dy == 0) ? 1e30 : fabs(1.0 / ray_dy);

    // Calculate step and initial side_dist
    int step_x, step_y;
    double side_dist_x, side_dist_y;

    if (ray_dx < 0) {
        step_x = -1;
        side_dist_x = (ray_x - map_x) * delta_dist_x;
    } else {
        step_x = 1;
        side_dist_x = (map_x + 1.0 - ray_x) * delta_dist_x;
    }
    
    if (ray_dy < 0) {
        step_y = -1;
        side_dist_y = (ray_y - map_y) * delta_dist_y;
    } else {
        step_y = 1;
        side_dist_y = (map_y + 1.0 - ray_y) * delta_dist_y;
    }

    // Perform DDA
    int hit = 0;
    int side; // 0 = x-side, 1 = y-side
    double perpendicular_wall_dist;

    while (hit == 0) {
        // Jump to next map square, either in x-direction or y-direction
        if (side_dist_x < side_dist_y) {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            side = 0;
        } else {
            side_dist_y += delta_dist_y;
            map_y += step_y;
            side = 1;
        }

        // Check if ray has hit a wall or is out of bounds
        if (map_x < 0 || map_x >= game->width || map_y < 0 || map_y >= game->height) {
            // Hit boundary, calculate distance to boundary
            if (side == 0)
                perpendicular_wall_dist = (map_x - ray_x + (1 - step_x) / 2) / ray_dx;
            else
                perpendicular_wall_dist = (map_y - ray_y + (1 - step_y) / 2) / ray_dy;
            break;
        }
        
        if (game->map[map_y][map_x] == '1') {
            hit = 1;
            // Calculate perpendicular distance to wall
            if (side == 0)
                perpendicular_wall_dist = (map_x - ray_x + (1 - step_x) / 2) / ray_dx;
            else
                perpendicular_wall_dist = (map_y - ray_y + (1 - step_y) / 2) / ray_dy;
        }
    }

    // Convert back to pixel distance
    return perpendicular_wall_dist * TILE;
}

// DDA version with ray visualization
void cast_single_ray_dda_visual(t_map *game, double ray_angle)
{
    // Starting position (convert from pixel coordinates to map coordinates)
    double ray_x = (game->player.player_x + PLAYER_OFFSET + PLAYER_SIZE/2) / (double)TILE;
    double ray_y = (game->player.player_y + PLAYER_OFFSET + PLAYER_SIZE/2) / (double)TILE;

    // Ray direction
    double ray_angle_rad = DEG_TO_RAD(ray_angle);
    double ray_dx = cos(ray_angle_rad);
    double ray_dy = sin(ray_angle_rad);

    // Current map position
    int map_x = (int)ray_x;
    int map_y = (int)ray_y;

    // Length of ray from current position to x-side or y-side
    double delta_dist_x = (ray_dx == 0) ? 1e30 : fabs(1.0 / ray_dx);
    double delta_dist_y = (ray_dy == 0) ? 1e30 : fabs(1.0 / ray_dy);

    // Calculate step and initial side_dist
    int step_x, step_y;
    double side_dist_x, side_dist_y;

    if (ray_dx < 0) {
        step_x = -1;
        side_dist_x = (ray_x - map_x) * delta_dist_x;
    } else {
        step_x = 1;
        side_dist_x = (map_x + 1.0 - ray_x) * delta_dist_x;
    }
    
    if (ray_dy < 0) {
        step_y = -1;
        side_dist_y = (ray_y - map_y) * delta_dist_y;
    } else {
        step_y = 1;
        side_dist_y = (map_y + 1.0 - ray_y) * delta_dist_y;
    }

    // Perform DDA with visualization
    int hit = 0;
    while (hit == 0) {
        // Jump to next map square
        if (side_dist_x < side_dist_y) {
            side_dist_x += delta_dist_x;
            map_x += step_x;
        } else {
            side_dist_y += delta_dist_y;
            map_y += step_y;
        }

        // Check if ray is out of bounds
        if (map_x < 0 || map_x >= game->width || map_y < 0 || map_y >= game->height) {
            break;
        }
        
        // Check for wall hit
        if (game->map[map_y][map_x] == '1') {
            hit = 1;
        } else {
            // Draw pixel at center of current tile (for visualization)
            int pixel_x = map_x * TILE + TILE/2;
            int pixel_y = map_y * TILE + TILE/2;
            mlx_pixel_put(game->mlx, game->win, pixel_x, pixel_y, 0xFFFF00);
        }
    }
}

// Cast all rays for 60-degree FOV using DDA
void cast_fov_rays_dda(t_map *game)
{
    double fov = 60.0;
    int window_width = game->width * TILE;
    int num_rays = window_width;
      
    double angle_step = fov / (double)num_rays;
    double start_angle = game->player.angle - (fov / 2.0);
    
    for (int i = 0; i < num_rays; i++)
    {
        double current_angle = start_angle + (i * angle_step);
        
        // Normalize angle to [0, 360) range
        while (current_angle < 0) 
            current_angle += 360.0;
        while (current_angle >= 360.0) 
            current_angle -= 360.0;
        
        // Get distance using DDA (for 3D rendering later)
        double distance = cast_single_ray_dda(game, current_angle);
        
        // Store distance for 3D rendering
        // game->ray_distances[i] = distance;
    }
}

// Cast fewer rays for visualization using DDA
void cast_fov_rays_dda_sparse(t_map *game, int ray_spacing)
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
        
        // Use visual version for sparse rays
        cast_single_ray_dda_visual(game, current_angle);
    }
}