/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.colors2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/18 17:25:42 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	parse_ceiling_color(t_map *map, char *line)
{
	if (map->ceiling_color)
	{
		printf("Error\nDuplicate C color found\n");
		return (0);
	}
	map->ceiling_color = extract_path(line);
	return (1);
}

int	parse_floor_color(t_map *map, char *line)
{
	if (map->floor_color)
	{
		printf("Error\nDuplicate F color found\n");
		return (0);
	}
	map->floor_color = extract_path(line);
	return (1);
}

int	validate_color(char *color, char *name, char *type)
{
	if (!color)
	{
		printf("Error: Missing %s (%s) color\n", name, type);
		return (0);
	}
	if (!validate_color_format(color))
	{
		printf("Error: Invalid %s color format: %s\n", type, color);
		return (0);
	}
	return (1);
}

int	validate_all_colors(t_map *map)
{
	int	valid;

	valid = 1;
	if (!validate_color(map->floor_color, "F", "Floor"))
		valid = 0;
	if (!validate_color(map->ceiling_color, "C", "Ceiling"))
		valid = 0;
	return (valid);
}
