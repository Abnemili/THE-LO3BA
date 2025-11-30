/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 20:18:00 by szaoual           #+#    #+#             */
/*   Updated: 2025/11/19 20:13:22 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"
#include "../../parsing/parsing.h"

int	extract_map_grid(t_map *map, t_map_bounds *bounds)
{
	int		map_lines;
	char	**grid;

	map_lines = bounds->last_map_line - bounds->start_idx + 1;
	grid = malloc(sizeof(char *) * (map_lines + 1));
	if (!grid)
	{
		printf("Error\nMemory allocation failed\n");
		return (0);
	}
	copy_map_lines(grid, map->map, bounds->start_idx, map_lines);
	free_unused_lines(map->map, bounds->start_idx,
		map->height, bounds->last_map_line);
	free(map->map);
	map->map = grid;
	map->height = map_lines;
	calculate_map_width(map, grid, map_lines);
	return (1);
}
