/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keys.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:03:40 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/18 19:52:24 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"

int	get_tile_color(t_map *map, int tile_y, int tile_x)
{
	if (map->map[tile_y][tile_x] == '1' || map->map[tile_y][tile_x] == ' ')
		return (COLOR_WALL);
	else if (map->map[tile_y][tile_x] == '0')
		return (COLOR_FREE);
	return (-1);
}

void	process_tile(t_map *map, int tile_x, int tile_y)
{
	int	color;

	if (tile_x >= 0 && tile_x < map->width
		&& tile_y >= 0 && tile_y < map->height)
	{
		if (map->map[tile_y] && map->map[tile_y][tile_x]
			&& map->map[tile_y][tile_x] != '\n'
			&& map->map[tile_y][tile_x] != '\r')
		{
			color = get_tile_color(map, tile_y, tile_x);
			if (color != -1)
				draw_square(map, tile_x, tile_y, color);
		}
	}
}

void	clear_player_area(t_map *map)
{
	int	x;
	int	y;
	int	tile_x;
	int	tile_y;

	x = -1;
	while (x <= 1)
	{
		y = -1;
		while (y <= 1)
		{
			tile_x = (map->player.player_x + PLAYER_OFFSET
					+ PLAYER_SIZE / 2) / TILE + x;
			tile_y = (map->player.player_y + PLAYER_OFFSET
					+ PLAYER_SIZE / 2) / TILE + y;
			process_tile(map, tile_x, tile_y);
			y++;
		}
		x++;
	}
}

void	free_textures(t_map *map)
{
	if (map->no_texture)
		free(map->no_texture);
	if (map->so_texture)
		free(map->so_texture);
	if (map->we_texture)
		free(map->we_texture);
	if (map->ea_texture)
		free(map->ea_texture);
	if (map->floor_color)
		free(map->floor_color);
	if (map->ceiling_color)
		free(map->ceiling_color);
}

void	free_map_array(t_map *map)
{
	int	i;

	if (map->map)
	{
		i = 0;
		while (i < map->height && map->map[i])
		{
			free(map->map[i]);
			i++;
		}
		free(map->map);
	}
}
