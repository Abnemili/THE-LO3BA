/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading_game.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:03:28 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/29 11:19:19 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"

void	pixel_put_img(t_map *map, int x, int y, int color)
{
	int	index;

	if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
		return ;
	index = y * map->img_size_line + x * (map->img_bpp / 8);
	*(unsigned int *)(map->img_data + index) = color;
}

void	draw_square_line(t_map *map, int x, int y, int dy)
{
	int	dx;

	dx = 0;
	while (dx < TILE)
	{
		pixel_put_img(map, x * TILE + dx, y * TILE + dy, map->img_endian);
		dx++;
	}
}

void	draw_square(t_map *map, int x, int y, int color)
{
	int	dy;

	(void)color;
	dy = 0;
	while (dy < TILE)
	{
		draw_square_line(map, x, y, dy);
		dy++;
	}
}

void	load_game(t_map *map)
{
	if (!map)
	{
		printf("Error: Invalid map structure\n");
		exit(1);
	}
	if (!init_mlx_connection(map))
		exit(1);
	if (!load_textures(map))
	{
		printf("Error: Failed to load textures\n");
		exit(1);
	}
	if (!init_rendering(map))
		exit(1);
	setup_player(map);// done 
	setup_events(map);// on work
	initial_render(map);
	mlx_loop(map->mlx);
}
