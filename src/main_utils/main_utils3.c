/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 20:17:00 by szaoual           #+#    #+#             */
/*   Updated: 2025/11/18 20:22:22 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"
#include "../../parsing/parsing.h"

int	check_map_line(char *s, t_map_bounds *bounds)
{
	if (s && (*s == '1' || *s == '0'))
	{
		if (bounds->found_empty)
			return (-1);
		bounds->last_map_line = bounds->end_idx;
	}
	else if (s && (*s == '\0' || *s == '\n' || *s == '\r'))
	{
		if (bounds->last_map_line >= bounds->start_idx)
			bounds->found_empty = 1;
	}
	else if (s && *s != '\0')
		return (0);
	return (1);
}

int	find_map_bounds(t_map *map, t_map_bounds *bounds)
{
	char	*s;
	int		result;

	bounds->end_idx = bounds->start_idx;
	bounds->last_map_line = bounds->start_idx;
	bounds->found_empty = 0;
	while (bounds->end_idx < map->height)
	{
		s = skip_spaces(map->map[bounds->end_idx]);
		result = check_map_line(s, bounds);
		if (result == -1)
		{
			printf("Error\nDuplicate map found at line %d\n", bounds->end_idx);
			return (0);
		}
		if (result == 0)
		{
			printf("Error\nInvalid content after map grid at line %d\n",
				bounds->end_idx);
			return (0);
		}
		bounds->end_idx++;
	}
	return (1);
}

void	copy_map_lines(char **grid, char **old_map, int start, int lines)
{
	int	k;

	k = 0;
	while (k < lines)
	{
		grid[k] = old_map[start + k];
		k++;
	}
	grid[lines] = NULL;
}

void	free_unused_lines(char **map, int start, int end, int last)
{
	int	k;

	k = 0;
	while (k < start)
	{
		free(map[k]);
		k++;
	}
	k = last + 1;
	while (k < end)
	{
		free(map[k]);
		k++;
	}
}
