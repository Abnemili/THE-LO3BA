/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading_game.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:03:28 by abnemili          #+#    #+#             */
/*   Updated: 2025/08/23 14:18:55 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"



void init_image(t_map *map)
{
    map->img = mlx_new_image(map->mlx, map->width * TILE, map->height * TILE);
    map->img_data = mlx_get_data_addr(map->img, &map->img_bpp, &map->img_size_line, &map->img_endian);
}

// Draw pixel onto image buffer instead of directly to window
void pixel_put_img(t_map *map, int x, int y, int color)
{
    int index;

    index = y * map->img_size_line + x * (map->img_bpp / 8);
    *(unsigned int *)(map->img_data + index) = color;
}

// Clear the entire image buffer
void clear_image(t_map *map)
{
    int x, y;
    for (y = 0; y < map->height * TILE; y++)
        for (x = 0; x < map->width * TILE; x++)
            pixel_put_img(map, x, y, COLOR_FREE); // black background
}

// Render the entire scene into the buffer
void render_scene(t_map *map)
{
    clear_image(map);            // Fill background
    set_color(map);              // Draw static map (walls & free spaces)
    draw_player(map);  
    mlx_put_image_to_window(map->mlx, map->win, map->img, 0, 0); // Show buffer
}

void setup_rendering(t_map *map)
{
    init_image(map);
}
// drawing the player 
void draw_player(t_map *map)
{
    int i;
    int j;

    i = 0;
    while (PLAYER_SIZE > i)
    {
        j = 0;
        while (PLAYER_SIZE > j)
        {
            mlx_pixel_put(map->mlx, map->win, 
                map->player.player_x  + PLAYER_OFFSET + i,
                map->player.player_y + PLAYER_OFFSET + j, 
                PLAYER_COLOR);
            j++;
        }
        i++;
    }
}

// seting the color in the actual block or tiles
void draw_square(t_map *map, int x, int y, int color)
{
    int dx;
    int dy;

    dy = 0;
    while (TILE > dy)
    {
        dx = 0;
        while (TILE > dx)
        {
            mlx_pixel_put(map->mlx, map->win,
                x * TILE + dx, // here
                y * TILE + dy, 
                color);
            dx++;
        }
        dy++;
    }
}

// colorint the free space and the walls
void set_color(t_map *map)
{
    int x;
    int y;
    int color;

    y = 0;
    while (map->height > y)
    {
        x = 0;
        while (map->width > x)
        {
            if(map->map[y][x] == 48)
                color = COLOR_FREE;
            else if (map->map[y][x] == 49)
                color = COLOR_WALL;
            else 
                continue;
            draw_square(map, x , y, color);
            x++;
        }
        y++;
    }
}


// init the player first position in the map
void init_player_position(t_map *map)
{
    int x;
    int y;

    y = 0;
   
    while (map->height > y)
    {
        x = 0;
        while (map->width > x)
        {
            if (map->map[y][x] == '0')
            {
                map->player.player_x = x * TILE + 4;
                map->player.player_y = y * TILE + 4;
                return;
            }
            x++;
        }
        y++;
    }
    map->player.player_x = 4;
    map->player.player_y = 4;
    map->player.angle = 0.0;
}


void  mlx_connection(t_map *map)
{
    map->mlx = mlx_init();
    if (!map->mlx)
    {
        exit(1);
    }
    map->win = mlx_new_window(map->mlx, map->width * TILE,
        map->height * TILE, "THE_LO3BA");
    if (!map->win)
    {
        mlx_destroy_display(map->mlx);
        free(map->mlx);
        exit(1);
    }
}

void load_game(t_map *map)
{
    mlx_connection(map); // done
    init_player_position(map); // done
 
    // Initial render
    set_color(map);  // done
    draw_player(map); // done
    // draw_ray(map);


    // Set up event hooks
    mlx_hook(map->win, 2, 1, handle_key_input, map);
    mlx_hook(map->win, 17, 0, handle_close, map);


    
    // Start game loop
    mlx_loop(map->mlx);
    
}
