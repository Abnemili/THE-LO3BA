/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/18 17:13:07 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	add_neighbor_to_stack(t_flood_data *data, t_point neighbor)
{
	if (!data->visited[neighbor.y][neighbor.x])
	{
		data->visited[neighbor.y][neighbor.x] = 1;
		data->stack[(*data->stack_size)++] = neighbor;
	}
}

int	process_neighbors(t_map *map, t_point p, t_flood_data *data)
{
	int	dx[4];
	int	dy[4];
	int	i;
	int	nx;
	int	ny;

	init_directions(dx, dy);
	i = 0;
	while (i < 4)
	{
		nx = p.x + dx[i];
		ny = p.y + dy[i];
		if (check_boundary_escape(map, nx, ny))
		{
			*data->stack_size = 0;
			return (0);
		}
		if (ny >= 0 && ny < map->height && nx >= 0 && nx < map->width)
			add_neighbor_to_stack(data, (t_point){nx, ny});
		i++;
	}
	return (1);
}

t_point	*allocate_stack(t_map *map, char **visited)
{
	t_point	*stack;

	stack = malloc(sizeof(t_point) * (map->width * map->height));
	if (!stack)
	{
		free_visited(visited, map);
		return (NULL);
	}
	return (stack);
}

char	*skip_to_comma(char *ptr)
{
	while (*ptr && (*ptr == ' ' || *ptr == '\t'))
		ptr++;
	if (*ptr != ',')
		return (NULL);
	return (ptr + 1);
}

int	validate_parsing_completeness(t_map *map)
{
	int	valid_textures;
	int	valid_colors;

	valid_textures = validate_all_textures(map);
	valid_colors = validate_all_colors(map);
	return (valid_textures && valid_colors);
}
