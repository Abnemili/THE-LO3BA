/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading_game.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:03:28 by abnemili          #+#    #+#             */
/*   Updated: 2025/08/19 12:03:29 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"

void draw_player(t_map *map)
{
    int i, j;
    
    for (i = 0; i < PLAYER_SIZE; i++)
    {
        for (j = 0; j < PLAYER_SIZE; j++)
        {
            mlx_pixel_put(map->mlx, map->win,
                map->player_x + PLAYER_OFFSET + i,
                map->player_y + PLAYER_OFFSET + j,
                PLAYER_COLOR);
        }
    }
}

void draw_square(t_map *map, int x, int y, int color)
{
    int dx, dy;
    
    for (dy = 0; dy < TILE; dy++)
    {
        for (dx = 0; dx < TILE; dx++)
        {
            mlx_pixel_put(map->mlx, map->win,
                x * TILE + dx,
                y * TILE + dy,
                color);
        }
    }
}

void set_color(t_map *map)
{
    int x, y;
    int color;
    
    for (y = 0; y < map->y; y++)
    {
        for (x = 0; x < map->x; x++)
        {
            if (map->map[y][x] == '1')
                color = COLOR_WALL;
            else if (map->map[y][x] == '0')
                color = COLOR_FREE;
            else
                continue;
            draw_square(map, x, y, color);
        }
    }
}

void init_player_position(t_map *map)
{
    int x, y;
    
    // Find first free space ('0') to place player
    for (y = 0; y < map->y; y++)
    {
        for (x = 0; x < map->x; x++)
        {
            if (map->map[y][x] == '0')
            {
                // Place player at a simple position within the tile
                map->player_x = x * TILE + 4;
                map->player_y = y * TILE + 4;
                return;
            }
        }
    }
    
    // Fallback: place at origin
    map->player_x = 4;
    map->player_y = 4;
}

void mlx_connection(t_map *map)
{
    map->mlx = mlx_init();
    if (!map->mlx)
    {
        exit(1);
    }
    map->win = mlx_new_window(map->mlx, map->x * TILE,
        map->y * TILE, "THE_LO3BA");
    if (!map->win)
    {
        mlx_destroy_display(map->mlx);
        free(map->mlx);
        exit(1);
    }
}

void load_game(t_map *map)
{
    mlx_connection(map);
    init_player_position(map);
    
    // Initial render
    set_color(map);
    draw_player(map);
    
    // Set up event hooks
    mlx_hook(map->win, 2, 1, handle_key_input, map);
    mlx_hook(map->win, 17, 0, handle_close, map);
    
    // Start game loop
    mlx_loop(map->mlx);
}
