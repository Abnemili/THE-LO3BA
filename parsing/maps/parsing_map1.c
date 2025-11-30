/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/19 20:13:22 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

void	scan_map_for_player(t_map *map, int *player_count, int *px, int *py)
{
	int	y;
	int	x;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (map->map[y][x] && map->map[y][x] != '\n'
			&& map->map[y][x] != '\r')
		{
			process_map_char(map, x, y, player_count);
			if (*player_count == 1 && *px == -1)
			{
				*px = x;
				*py = y;
			}
			x++;
		}
		y++;
	}
}

int	validate_and_set_player(t_map *map)
{
	int	player_count;
	int	player_x;
	int	player_y;

	if (!map || !map->map)
		return (0);
	player_count = 0;
	player_x = -1;
	player_y = -1;
	scan_map_for_player(map, &player_count, &player_x, &player_y);
	if (player_count == 0)
	{
		printf("Error\nNo player spawn (N/S/W/E) found in map\n");
		return (0);
	}
	if (!check_map_closed(map, player_x, player_y))
	{
		printf("Error\nMap is not closed - player can escape to the outside\n");
		return (0);
	}
	return (1);
}

char	get_map_char(t_map *map, int x, int y)
{
	int	len;

	if (y < 0 || y >= map->height)
		return ('\0');
	if (!map->map[y])
		return ('\0');
	if (x < 0)
		return ('\0');
	len = 0;
	while (map->map[y][len] && map->map[y][len] != '\n'
		&& map->map[y][len] != '\r')
		len++;
	if (x >= len)
		return ('\0');
	return (map->map[y][x]);
}

int	check_boundary_escape(t_map *map, int nx, int ny)
{
	char	nc;

	if (ny < 0 || ny >= map->height || nx < 0 || nx >= map->width)
	{
		nc = get_map_char(map, nx, ny);
		if (nc == '\0')
			return (1);
	}
	return (0);
}

int	flood_fill_loop(t_map *map, t_flood_data *data)
{
	t_point	p;
	char	c;

	while (*data->stack_size > 0)
	{
		p = data->stack[--(*data->stack_size)];
		c = get_map_char(map, p.x, p.y);
		if (c == '\0')
			return (0);
		if (c == '1')
			continue ;
		if (!process_neighbors(map, p, data))
			return (0);
	}
	return (1);
}
