#include "header/the_lo3ba.h"

void draw_pixel(t_map *game, int x, int y, int color)
{
    // Check if the coordinates are within the drawable area to avoid out-of-bounds access
    if (x < 0 || y < 0 || x >= game->width * TILE
            || y >= game->height * TILE)
        return;

    // Calculate the byte offset in the image data for the pixel at (x, y)
    // img_size_line = number of bytes in one image row (including padding if any)
    // img_bpp = bits per pixel, dividing by 8 converts it to bytes per pixel
    int index = (y * game->img_size_line) + (x * (game->img_bpp / 8));

    // Get a pointer to the pixel's memory location inside the image buffer
    unsigned int *pixel = (unsigned int *)(game->img_data + index);

    // Set the pixel color at this location
    *pixel = color;
}

int handle_close(t_map *map)
{
    if (map->win)
        mlx_destroy_window(map->mlx, map->win);
    if (map->mlx)
    {
        mlx_destroy_display(map->mlx);
        free(map->mlx);
    }
    exit(0);
    return (0);
}
int is_valid_move(t_map *map, int new_x, int new_y)
{
    int tile_x, tile_y;
    
    // Simple approach: just check the center tile of the player
    tile_x = (new_x + PLAYER_OFFSET + PLAYER_SIZE/2) / TILE;
    tile_y = (new_y + PLAYER_OFFSET + PLAYER_SIZE/2) / TILE;
    
    // Check bounds
    if (tile_x < 0 || tile_x >= map->width ||
        tile_y < 0 || tile_y >= map->height)
        return (0);
    
    // Check if center tile is free (not a wall)
    if (map->map[tile_y][tile_x] == '1')
        return (0);
    
    return (1);
}

void cast_single_ray(t_map *game, double ray_angle)
{
    double px = game->player.player_x + PLAYER_OFFSET + PLAYER_SIZE / 2;
    double py = game->player.player_y + PLAYER_OFFSET + PLAYER_SIZE / 2;

    double ray_angle_rad = DEG_TO_RAD(ray_angle);

    double ray_dir_x = cos(ray_angle_rad);
    double ray_dir_y = sin(ray_angle_rad);

    int map_x = (int)(px / TILE);
    int map_y = (int)(py / TILE);

    // Step and sideDist for DDA
    double delta_dist_x = fabs(1 / ray_dir_x);
    double delta_dist_y = fabs(1 / ray_dir_y);

    double side_dist_x;
    double side_dist_y;

    int step_x;
    int step_y;

    int hit = 0;      // Was a wall hit?
    int side = 0;     // Was it a vertical (0) or horizontal (1) wall?

    // Calculate step and initial side_dist
    if (ray_dir_x < 0)
    {
        step_x = -1;
        side_dist_x = (px - map_x * TILE) * delta_dist_x / TILE;
    }
    else
    {
        step_x = 1;
        side_dist_x = ((map_x + 1) * TILE - px) * delta_dist_x / TILE;
    }

    if (ray_dir_y < 0)
    {
        step_y = -1;
        side_dist_y = (py - map_y * TILE) * delta_dist_y / TILE;
    }
    else
    {
        step_y = 1;
        side_dist_y = ((map_y + 1) * TILE - py) * delta_dist_y / TILE;
    }

    while (!hit)
    {
        if (side_dist_x < side_dist_y)
        {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            side = 0;
        }
        else
        {
            side_dist_y += delta_dist_y;
            map_y += step_y;
            side = 1;
        }

        if (map_x < 0 || map_y < 0 || map_y >= game->height || map_x >= game->width)
            break;

        if (game->map[map_y][map_x] == '1')
            hit = 1;
    }

    // Calculate hit point
    double hit_x, hit_y;
    if (side == 0)
    {
        hit_x = map_x * TILE;
        hit_y = py + (side_dist_x - delta_dist_x) * ray_dir_y * TILE;
    }
    else
    {
        hit_y = map_y * TILE;
        hit_x = px + (side_dist_y - delta_dist_y) * ray_dir_x * TILE;
    }

    // Draw ray
    for (int i = 0; i < 100; i++)
    {
        int draw_x = (int)(px + ray_dir_x * i);
        int draw_y = (int)(py + ray_dir_y * i);
        if (draw_x < 0 || draw_y < 0 || draw_x >= game->width * TILE || draw_y >= game->height * TILE)
            break;
        draw_pixel(game, draw_x, draw_y, 0xFFFF00);
    }

    // Optionally store hit direction for shading
    char *dir = "";
    if (side == 0)
        dir = (ray_dir_x < 0) ? "W" : "E";
    else
        dir = (ray_dir_y < 0) ? "N" : "S";

    printf("Ray hit at tile (%d,%d) | Direction: %s\n", map_x, map_y, dir);
}

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
    double new_x = map->player.player_x;
    double new_y = map->player.player_y;
    double rad = DEG_TO_RAD(map->player.angle);
    double move_step = MOVE_SPEED;

    if (keycode == KEY_ESC) // Escape key pressed
        return handle_close(map);

    // Handle rotation keys (Left & Right arrows)
    if (keycode == 65361) // Left arrow key
    {
        map->player.angle -= 5.0;
        if (map->player.angle < 0)
            map->player.angle += 360.0;
    }
    else if (keycode == 65363) // Right arrow key
    {
        map->player.angle += 5.0;
        if (map->player.angle >= 360.0)
            map->player.angle -= 360.0;
    }
    // Movement keys (WASD)
    else if (keycode == KEY_W) // Move forward
    {
        new_x += cos(rad) * move_step;
        new_y += sin(rad) * move_step;
    }
    else if (keycode == KEY_S) // Move backward
    {
        new_x -= cos(rad) * move_step;
        new_y -= sin(rad) * move_step;
    }
    else if (keycode == KEY_A) // Strafe left
    {
        new_x += cos(rad - M_PI / 2) * move_step;
        new_y += sin(rad - M_PI / 2) * move_step;
    }
    else if (keycode == KEY_D) // Strafe right
    {
        new_x += cos(rad + M_PI / 2) * move_step;
        new_y += sin(rad + M_PI / 2) * move_step;
    }
    else
        return 0; // No relevant key pressed

    // Check valid move before updating position
    if (is_valid_move(map, (int)new_x, (int)new_y))
    {
        // Update player position
        map->player.player_x = new_x;
        map->player.player_y = new_y;
    }

    // Clear window and redraw everything after movement or rotation
    mlx_clear_window(map->mlx, map->win);
    set_color(map);
    draw_player(map);
    cast_fov_rays_sparse(map, ray_num);

    return 0;
}


