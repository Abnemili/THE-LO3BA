#include "../header/the_lo3ba.h"


void draw_pixel(t_map *game, int x, int y, int color)
{
    if (x < 0 || y < 0 || x >= game->width * TILE || y >= game->height * TILE)
        return;
    
    int index = (y * game->img_size_line) + (x * (game->img_bpp / 8));
    unsigned int *pixel = (unsigned int *)(game->img_data + index);
    *pixel = color;
}
void draw_ray(t_map *game)
{
    double ray_x = game->player.player_x + PLAYER_OFFSET + PLAYER_SIZE/2;
    double ray_y = game->player.player_y + PLAYER_OFFSET + PLAYER_SIZE/2;

    printf("Player pos: %.2f, %.2f\n", ray_x, ray_y);
    printf("Player angle: %.2f\n", game->player.angle);

    double ray_angle_rad = DEG_TO_RAD(game->player.angle);
    double ray_dx = cos(ray_angle_rad);
    double ray_dy = sin(ray_angle_rad);

    printf("Ray direction: %.2f, %.2f\n", ray_dx, ray_dy);

    double step_size = 0.5;
    int max_depth = 500;
    int i = 0;
    int pixels_drawn = 0;

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
            pixels_drawn++;
        }

        ray_x += ray_dx * step_size;
        ray_y += ray_dy * step_size;
        i++;
    }
    
    printf("Pixels drawn: %d\n", pixels_drawn);
}