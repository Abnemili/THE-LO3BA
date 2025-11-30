/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading_game3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 18:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/26 16:57:59 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"

int	check_player_position(t_map *map, int x, int y)
{
	if (map->map[y][x] && map->map[y][x] != '\n'
		&& map->map[y][x] != '\r' && map->map[y][x] == '0')
	{
		map->player.player_x = x * TILE + (TILE / 2);
		map->player.player_y = y * TILE + (TILE / 2);
		map->player.angle = 0.0;
		return (1);
	}
	return (0);
}

int	find_player_position(t_map *map)
{
	int	y;
	int	x;

	y = 0;
	while (y < map->height)
	{
		if (!map->map[y])
		{
			y++;
			continue ;
		}
		x = 0;
		while (x < map->width)
		{
			if (check_player_position(map, x, y))
				return (1);
			x++;
		}
		y++;
	}
	return (0);
}

void	setup_player(t_map *map)
{
	if (map->player_set)
		return ;
	if (!find_player_position(map)) // if we faile to find the player on the map we set it manualy.
	{
		map->player.player_x = TILE / 2;
		map->player.player_y = TILE / 2;
		map->player.angle = 0.0;
	}
}
