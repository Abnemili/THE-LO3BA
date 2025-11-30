/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/18 17:30:39 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	parse_map_config(t_map *map)
{
	int		i;
	char	*line;
	int		result;

	if (!map || !map->map)
		return (0);
	i = 0;
	while (map->map[i])
	{
		line = skip_spaces(map->map[i]);
		if (!*line || *line == '\n')
		{
			i++;
			continue ;
		}
		result = parse_config_line(map, line);
		if (result == 0)
			return (0);
		if (result == -1)
			break ;
		i++;
	}
	return (1);
}

int	check_first_row(t_map *map)
{
	int		x;
	char	c;

	x = 0;
	while (map->map[0][x] && map->map[0][x] != '\n'
		&& map->map[0][x] != '\r')
	{
		c = map->map[0][x];
		if (c != '1' && c != ' ' && c != '\t')
		{
			printf("Error\nFirst row must only contain walls\n");
			return (0);
		}
		x++;
	}
	return (1);
}

int	check_last_row(t_map *map)
{
	int		x;
	char	c;

	x = 0;
	while (map->map[map->height - 1][x] && map->map[map->height - 1][x] != '\n'
		&& map->map[map->height - 1][x] != '\r')
	{
		c = map->map[map->height - 1][x];
		if (c != '1' && c != ' ' && c != '\t')
		{
			printf("Error\nLast row must only contain walls ('1') or spaces\n");
			return (0);
		}
		x++;
	}
	return (1);
}

int	get_row_length(char *row)
{
	int	len;

	len = 0;
	while (row[len] && row[len] != '\n' && row[len] != '\r')
		len++;
	return (len);
}

int	find_first_nonspace(char *row, int len)
{
	int	first_char;

	first_char = 0;
	while (first_char < len && (row[first_char] == ' '
			|| row[first_char] == '\t'))
		first_char++;
	return (first_char);
}
