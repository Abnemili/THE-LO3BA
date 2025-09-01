#include "header/the_lo3ba.h"

// Draw a pixel at (x, y) with the given color on the game image
void draw_pixel(t_map *game, int x, int y, int color)
{

    if (x < 0 || y < 0 || x >= game->width * TILE
            || y >= game->height * TILE)
        return;
    int index = (y * game->img_size_line) + (x * (game->img_bpp / 8));

    unsigned int *pixel = (unsigned int *)(game->img_data + index);


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

    double step_size = 0.7;
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
            printf("Finished casting all rays\n");
    }
}
 