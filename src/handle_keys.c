/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keys.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:03:40 by abnemili          #+#    #+#             */
/*   Updated: 2025/08/19 12:03:41 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"


int is_valid_move(t_map *map, int new_x, int new_y)
{
    int tile_x, tile_y;
    
    // Simple approach: just check the center tile of the player
    tile_x = (new_x + PLAYER_OFFSET + PLAYER_SIZE/2) / TILE;
    tile_y = (new_y + PLAYER_OFFSET + PLAYER_SIZE/2) / TILE;
    
    // Check bounds
    if (tile_x < 0 || tile_x >= map->x ||
        tile_y < 0 || tile_y >= map->y)
        return (0);
    
    // Check if center tile is free (not a wall)
    if (map->map[tile_y][tile_x] == '1')
        return (0);
    
    return (1);
}

void clear_player_area(t_map *map)
{
    int x, y;
    
    // Simply redraw a small area around the player
    for (y = -1; y <= 1; y++)
    {
        for (x = -1; x <= 1; x++)
        {
            int tile_x = (map->player_x + PLAYER_OFFSET + PLAYER_SIZE/2) / TILE + x;
            int tile_y = (map->player_y + PLAYER_OFFSET + PLAYER_SIZE/2) / TILE + y;
            
            if (tile_x >= 0 && tile_x < map->x && tile_y >= 0 && tile_y < map->y)
            {
                int color;
                if (map->map[tile_y][tile_x] == '1')
                    color = COLOR_WALL;
                else if (map->map[tile_y][tile_x] == '0')
                    color = COLOR_FREE;
                else
                    continue;
                draw_square(map, tile_x, tile_y, color);
            }
        }
    }
}

int handle_key_input(int keycode, t_map *map)
{
    int new_x, new_y;
    
    if (keycode == KEY_ESC)
        return (handle_close(map));
    
    // Calculate new position
    new_x = map->player_x;
    new_y = map->player_y;
    
    if (keycode == KEY_W) // W - Up
        new_y -= MOVE_SPEED;
    else if (keycode == KEY_S) // S - Down
        new_y += MOVE_SPEED;
    else if (keycode == KEY_A) // A - Left
        new_x -= MOVE_SPEED;
    else if (keycode == KEY_D) // D - Right
        new_x += MOVE_SPEED;
    else
        return (0); // Unknown key, do nothing
    
    // Check if move is valid
    if (is_valid_move(map, new_x, new_y))
    {
        // Clear previous player position
        clear_player_area(map);
        
        // Update position
        map->player_x = new_x;
        map->player_y = new_y;
        
        // Draw player at new position
        draw_player(map);
    }
    
    return (0);
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
