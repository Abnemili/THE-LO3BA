#include "../header/the_lo3ba.h"

#define DEG_TO_RAD(angleDegrees) ((angleDegrees) * 3.14159265358979323846 / 180.0)

// void draw_pixel(t_map *game, int x, int y, int color)
// {
//     if (x < 0 || y < 0 || x >= game->width * TILE || y >= game->height * TILE)
//         return;
    
//     int index = (y * game->img_size_line) + (x * (game->img_bpp / 8));
//     unsigned int *pixel = (unsigned int *)(game->img_data + index);
//     *pixel = color;
// }


void draw_pixel(t_map *game, int x, int y, int color)
{
    if (!game || !game->img_data)
    {
        fprintf(stderr, "[draw_pixel] NULL game or img_data pointer\n");
        return;
    }

    // Check pixel bounds within window size
    if (x < 0 || y < 0 || x >= game->width * TILE || y >= game->height * TILE)
    {
        // Outside image, just ignore
        return;
    }

    int bytes_per_pixel = game->img_bpp / 8;
    int index = y * game->img_size_line + x * bytes_per_pixel;

    // Calculate total image data size (in bytes)
    int max_bytes = game->img_size_line * (game->height * TILE);

    // Prevent buffer overflow
    if (index < 0 || index + bytes_per_pixel > max_bytes)
    {
        fprintf(stderr, "[draw_pixel] Pixel index out of bounds: %d\n", index);
        return;
    }

    // Write pixel color (assuming 32 bits per pixel)
    unsigned int *pixel = (unsigned int *)(game->img_data + index);
    *pixel = color;
}



void draw_ray(t_map *game)
{
    if (!game || !game->player || !game->map)
    {
        fprintf(stderr, "[draw_ray] NULL pointers detected\n");
        return;
    }

    double ray_x = game->player->player_x;
    double ray_y = game->player->player_y;

    double angle = game->player->angle;
    while (angle < 0) angle += 360;
    while (angle >= 360) angle -= 360;

    double ray_angle_rad = DEG_TO_RAD(angle);

    double ray_dx = cos(ray_angle_rad);
    double ray_dy = sin(ray_angle_rad);

    double step_size = 1.0;
    int max_depth = 500; // max pixels to trace along the ray

    for (int i = 0; i < max_depth; i++)
    {
        ray_x += ray_dx * step_size;
        ray_y += ray_dy * step_size;

        int map_x = (int)(ray_x / TILE);
        int map_y = (int)(ray_y / TILE);

        // Check map boundaries
        if (map_x < 0 || map_y < 0 || map_y >= game->height || map_x >= game->width)
        {
            // Out of map bounds — stop the ray here
            break;
        }

        // Safety: check if the map row pointer is valid
        if (!game->map[map_y])
        {
            fprintf(stderr, "[draw_ray] Null map row at %d\n", map_y);
            break;
        }

        // Stop at walls (assuming '1' means wall)
        if (game->map[map_y][map_x] == '1')
            break;

        // Draw pixel for the ray's current position
        draw_pixel(game, (int)ray_x, (int)ray_y, 0xFFFF00); // Yellow
    }
}
 
// void draw_ray(t_map *game)
// {
//     double ray_x = game->player->player_x;
//     double ray_y = game->player->player_y;

//     double ray_angle_rad = DEG_TO_RAD(game->player->angle);

//     double ray_dx = cos(ray_angle_rad);
//     double ray_dy = sin(ray_angle_rad);

//     double step_size = 1.0;
//     int max_depth = 500; // how far the ray goes
//     int i = 0;

//     while (i < max_depth)
//     {
//         ray_x += ray_dx * step_size;
//         ray_y += ray_dy * step_size;

//         int map_x = (int)(ray_x / TILE);
//         int map_y = (int)(ray_y / TILE);

//         // Check map boundaries
//         if (map_x < 0 || map_y < 0 || map_y >= game->height || map_x >= game->width)
//             break;

//         // Stop at wall
//         if (game->map[map_y][map_x] == '1')
//             break;

//         // Draw pixel at ray position
//         draw_pixel(game, (int)ray_x, (int)ray_y, 0xFFFF00); // Yellow ray
//         i++;
//     }
// }
