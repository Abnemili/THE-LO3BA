/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 20:19:00 by szaoual           #+#    #+#             */
/*   Updated: 2025/11/19 20:13:22 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"
#include "../../parsing/parsing.h"

int	init_map_struct(t_map **map)
{
	*map = malloc(sizeof(t_map));
	if (!*map)
	{
		printf("Error\nMemory allocation failed\n");
		return (0);
	}
	init_map_pointers(*map);
	init_map_keys(*map);
	return (1);
}

int	process_map_extraction(t_map *map)
{
	t_map_bounds	bounds;
	int				start_idx;

	map->floor_rgb = parse_rgb_color(map->floor_color);
	map->ceiling_rgb = parse_rgb_color(map->ceiling_color);
	start_idx = find_map_start(map);
	if (!validate_map_position(map, start_idx))
		return (0);
	bounds.start_idx = start_idx;
	if (!find_map_bounds(map, &bounds))
		return (0);
	if (!extract_map_grid(map, &bounds))
		return (0);
	return (1);
}

int	validate_all(t_map *map, char *filename)
{
	if (!check_and_open_file(filename, map)
		|| !read_file(map, filename)
		|| !parse_map_config(map)
		|| !validate_parsing_completeness(map)
		|| !process_map_extraction(map)
		|| !check_map_borders(map)
		|| !validate_and_set_player(map))
		return (0);
	return (1);
}
