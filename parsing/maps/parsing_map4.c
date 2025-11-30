/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/19 20:13:22 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	check_map_closed(t_map *map, int start_x, int start_y)
{
	t_flood_data	data;
	int				stack_size;
	int				result;

	data.visited = allocate_visited(map);
	if (!data.visited)
		return (0);
	data.stack = allocate_stack(map, data.visited);
	if (!data.stack)
		return (0);
	stack_size = 0;
	data.stack_size = &stack_size;
	data.stack[stack_size++] = (t_point){start_x, start_y};
	data.visited[start_y][start_x] = 1;
	result = flood_fill_loop(map, &data);
	free(data.stack);
	free_visited(data.visited, map);
	return (result);
}

void	cleanup_map_array(t_map *map)
{
	int	i;

	if (!map->map)
		return ;
	i = 0;
	while (i < map->height && map->map[i])
	{
		free(map->map[i]);
		i++;
	}
	free(map->map);
}

int	check_map_borders(t_map *map)
{
	if (!map || !map->map || map->height == 0)
		return (0);
	if (!check_first_row(map))
		return (0);
	if (!check_last_row(map))
		return (0);
	if (!check_middle_rows(map))
		return (0);
	return (1);
}

void	process_map_char(t_map *map, int x, int y, int *player_count)
{
	char	c;

	c = map->map[y][x];
	if (c == '1' || c == '0')
		return ;
	else if (c == ' ' || c == '\t')
		map->map[y][x] = '0';
	else if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
		handle_player_spawn(map, x, y, player_count);
	else
	{
		printf("Error\nInvalid character '%c' in map at (%d,%d)\n", c, y, x);
		exit(1);
	}
}
