/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keys2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:03:40 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/18 19:52:24 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"

void	destroy_mlx_resources(t_map *map)
{
	if (map->north_tex.img)
		mlx_destroy_image(map->mlx, map->north_tex.img);
	if (map->south_tex.img)
		mlx_destroy_image(map->mlx, map->south_tex.img);
	if (map->west_tex.img)
		mlx_destroy_image(map->mlx, map->west_tex.img);
	if (map->east_tex.img)
		mlx_destroy_image(map->mlx, map->east_tex.img);
	if (map->img)
		mlx_destroy_image(map->mlx, map->img);
	if (map->win)
		mlx_destroy_window(map->mlx, map->win);
	mlx_destroy_display(map->mlx);
	free(map->mlx);
}

int	handle_close(t_map *map)
{
	if (!map)
		exit(0);
	free_textures(map);
	free_map_array(map);
	if (map->mlx)
		destroy_mlx_resources(map);
	free(map);
	exit(0);
	return (0);
}

int	is_valid_move(t_map *map, int new_x, int new_y)
{
	int	tile_x;
	int	tile_y;

	tile_x = (new_x + PLAYER_OFFSET + PLAYER_SIZE / 2) / TILE;
	tile_y = (new_y + PLAYER_OFFSET + PLAYER_SIZE / 2) / TILE;
	if (tile_x < 0 || tile_x >= map->width
		|| tile_y < 0 || tile_y >= map->height)
		return (0);
	if (!map->map[tile_y] || !map->map[tile_y][tile_x]
		|| map->map[tile_y][tile_x] == '\n'
		|| map->map[tile_y][tile_x] == '\r')
		return (0);
	if (map->map[tile_y][tile_x] == '1' || map->map[tile_y][tile_x] == ' ')
		return (0);
	return (1);
}
