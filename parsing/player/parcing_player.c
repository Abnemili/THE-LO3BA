/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing_player.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/19 20:13:22 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

void	set_player_angle(t_map *map, char c)
{
	if (c == 'N')
		map->player.angle = 270.0;
	else if (c == 'S')
		map->player.angle = 90.0;
	else if (c == 'W')
		map->player.angle = 180.0;
	else
		map->player.angle = 0.0;
}

void	handle_player_spawn(t_map *map, int x, int y, int *player_count)
{
	(*player_count)++;
	if (*player_count > 1)
	{
		printf("Error\nMap contains more than one player spawn\n");
		exit(1);
	}
	map->player.player_x = x * TILE + TILE / 2;
	map->player.player_y = y * TILE + TILE / 2;
	set_player_angle(map, map->map[y][x]);
	map->map[y][x] = '0';
	map->player_set = 1;
}

char	*extract_path(char *line)
{
	char	*end;
	int		len;

	if (!line)
		return (NULL);
	while (*line && *line != ' ' && *line != '\t')
		line++;
	line = skip_spaces(line);
	if (!*line)
		return (NULL);
	end = line;
	while (*end && *end != '\n' && *end != '\r')
		end++;
	end = trim_end_spaces(line, end);
	len = end - line;
	if (len <= 0)
		return (NULL);
	return (copy_string(line, len));
}

void	init_directions(int *dx, int *dy)
{
	dx[0] = 0;
	dx[1] = 1;
	dx[2] = 0;
	dx[3] = -1;
	dy[0] = -1;
	dy[1] = 0;
	dy[2] = 1;
	dy[3] = 0;
}
