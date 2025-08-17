/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 14:24:40 by abnemili          #+#    #+#             */
/*   Updated: 2025/08/17 18:05:58 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int close_window(t_Map *map)
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

int key_press(int keycode, t_Map *map)
{
    if (keycode == 65307)  //ESC key
        close_window(map);
    printf("the ESC key was pressed");
    return (0);
}

void init_map(t_Map *map)
{
    map->mlx = mlx_init();
    if (!map->mlx)
    {
        printf("Error: init map failed\n");
        exit(1);
    }
    map->win = mlx_new_window(map->mlx, map->x * 32, map->y * 32, "THE LO3BA");
    if (!map->win)
    {
        printf("Error: opening window failed\n");
        mlx_destroy_display(map->mlx);
        free(map->mlx);
        exit(1);
    }
    
}
int srender(void *param)
{
    t_Map *player = (t_Map *)param;

    // Clear the window (if needed)
    mlx_clear_window(player->mlx, player->win);

    // Draw something, e.g., a pixel at player's position
    int px = (int)(player->x * 64); // assuming each tile is 64 pixels
    int py = (int)(player->y * 64);
    mlx_pixel_put(player->mlx, player->win, px, py, 0xFFFFFF); // white pixel

    return (0);
}


int main(int ac, char **av)
{
    t_Map *map;
    t_Map player;

    (void)ac;
    map = fill_map(av[1]);
    init_map(map);
    set_color(map, map->x, map->y);

    find_player(map, &player);

    // Create window if not already created
    map->mlx = mlx_init();
    map->win = mlx_new_window(map->mlx, 800, 600, "My Game");

    // Setup rendering loop
    mlx_loop_hook(map->mlx, (void *)srender, &player);

    // Event hooks
    mlx_hook(map->win, 17, 0, close_window, map);
    mlx_hook(map->win, 2, 1L<<0, key_press, map);

    mlx_loop(map->mlx);

    return (0);
}
