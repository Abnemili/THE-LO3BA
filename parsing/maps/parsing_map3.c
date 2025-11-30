/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/18 17:35:51 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	find_last_nonspace(char *row, int len)
{
	int	last_char;

	last_char = len - 1;
	while (last_char >= 0 && (row[last_char] == ' '
			|| row[last_char] == '\t'))
		last_char--;
	return (last_char);
}

int	check_row_borders(t_map *map, int y, int first_char, int last_char)
{
	if (first_char < get_row_length(map->map[y])
		&& map->map[y][first_char] != '1')
	{
		printf("Error\nRow %d must start with a wall ('1'), found '%c'\n",
			y, map->map[y][first_char]);
		return (0);
	}
	if (last_char >= 0 && map->map[y][last_char] != '1')
	{
		printf("Error\nRow %d must end with a wall ('1'), found '%c'\n",
			y, map->map[y][last_char]);
		return (0);
	}
	return (1);
}

int	should_skip_row(t_map *map, int y, int *len)
{
	if (!map->map[y] || !*map->map[y])
		return (1);
	*len = get_row_length(map->map[y]);
	if (*len == 0)
		return (1);
	return (0);
}

int	validate_row_borders(t_map *map, int y, int len)
{
	int	first_char;
	int	last_char;

	first_char = find_first_nonspace(map->map[y], len);
	last_char = find_last_nonspace(map->map[y], len);
	if (!check_row_borders(map, y, first_char, last_char))
		return (0);
	return (1);
}

int	check_middle_rows(t_map *map)
{
	int	y;
	int	len;

	y = 0;
	while (y < map->height)
	{
		if (should_skip_row(map, y, &len))
		{
			y++;
			continue ;
		}
		if (!validate_row_borders(map, y, len))
			return (0);
		y++;
	}
	return (1);
}
