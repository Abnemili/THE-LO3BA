/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 20:16:00 by szaoual           #+#    #+#             */
/*   Updated: 2025/11/19 20:13:22 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"
#include "../../parsing/parsing.h"

void	init_map_pointers(t_map *map)
{
	map->map = NULL;
	map->map_fd = -1;
	map->no_texture = NULL;
	map->so_texture = NULL;
	map->we_texture = NULL;
	map->ea_texture = NULL;
	map->floor_color = NULL;
	map->ceiling_color = NULL;
	map->mlx = NULL;
	map->win = NULL;
	map->img = NULL;
	map->img_data = NULL;
}

void	init_map_keys(t_map *map)
{
	map->keys.w = 0;
	map->keys.a = 0;
	map->keys.s = 0;
	map->keys.d = 0;
	map->keys.left = 0;
	map->keys.right = 0;
}

int	find_map_start(t_map *map)
{
	int		start_idx;
	char	*s;

	start_idx = 0;
	while (start_idx < map->height)
	{
		s = skip_spaces(map->map[start_idx]);
		if (s && (*s == '1' || *s == '0'))
			break ;
		start_idx++;
	}
	return (start_idx);
}

int	validate_map_position(t_map *map, int start_idx)
{
	if (start_idx >= map->height)
	{
		printf("Error\nNo map grid found in file\n");
		return (0);
	}
	return (1);
}
