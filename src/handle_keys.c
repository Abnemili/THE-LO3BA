/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keys.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:03:40 by abnemili          #+#    #+#             */
/*   Updated: 2025/08/31 15:02:43 by abnemili         ###   ########.fr       */
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
    if (tile_x < 0 || tile_x >= map->width ||
        tile_y < 0 || tile_y >= map->height)
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
            int tile_x = (map->player.player_x + PLAYER_OFFSET + PLAYER_SIZE/2) / TILE + x;
            int tile_y = (map->player.player_y + PLAYER_OFFSET + PLAYER_SIZE/2) / TILE + y;
            
            if (tile_x >= 0 && tile_x < map->width && tile_y >= 0 && tile_y < map->height)
            {//d
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
    
    if (keycode == KEY_ESC)// the escape kety was pressed 
        return (handle_close(map));
    
    // Handle rotation first
    if (keycode == 65361) // Left arrow key
    {
        map->player.angle -= 5.0;
        if (map->player.angle < 0)
            map->player.angle += 360;
        
        // Clear and redraw for rotation
        // mlx_clear_window(map->mlx, map->win);
       // Instead of calling set_color() and draw_player() separately:
        render_scene(map);  // This clears, draws map, and draws player
        cast_fov_rays(map); // Then cast rays
        mlx_put_image_to_window(map->mlx, map->win, map->img, 0, 0); // Show final result
        return (0);
    }
    else if (keycode == 65363) // Right arrow key
    {
        map->player.angle += 5.0;
        if (map->player.angle >= 360)
            map->player.angle -= 360;
        
        // Clear and redraw for rotation  
        // mlx_clear_window(map->mlx, map->win);
            // Instead of calling set_color() and draw_player() separately:
        render_scene(map);  // This clears, draws map, and draws player
        cast_fov_rays(map); // Then cast rays
        mlx_put_image_to_window(map->mlx, map->win, map->img, 0, 0); // Show final result
        return (0);
    }                                                                                       
    
    // Calculate new position for movement
    new_x = map->player.player_x;
    new_y = map->player.player_y;
    
    if (keycode == KEY_W) // W - Up
        new_y -= MOVE_SPEED;
    else if (keycode == KEY_S) // S - Down
        new_y += MOVE_SPEED;
    else if (keycode == KEY_A) // A - Left
        new_x -= MOVE_SPEED;
    else if (keycode == KEY_D) // D - Right
        new_x += MOVE_SPEED;
    else
        return (0);
    
    // Check if move is valid
    if (is_valid_move(map, new_x, new_y))
    {
        // Clear and redraw for movement
        // mlx_clear_window(map->mlx, map->win);
        
        // Update position
        map->player.player_x = new_x;
        map->player.player_y = new_y;
        
        render_scene(map);  // This clears, draws map, and draws player
        cast_fov_rays(map); // Then cast rays
        mlx_put_image_to_window(map->mlx, map->win, map->img, 0, 0); // Show final result

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
