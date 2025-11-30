/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 10:51:09 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/18 19:52:23 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"

void	validate_map(t_map *map)
{
	if (!map || !map->map || map->height <= 0)
	{
		puts("Error\nInvalid map structure\n");
		exit(1);
	}
	if (!map->map[0] || ft_strlen1(map->map[0]) <= 0)
	{
		puts("Error\nInvalid map content\n");
		exit(1);
	}
	printf("Map loaded: %dx%d\n", map->width, map->height);
}

t_map	*fill_map(char *av)
{
	t_map	*map;
	int		h;

	h = map_height(av);
	if (h < 1)
	{
		puts("Error\nThe map is invalid\n");
		exit(1);
	}
	map = init_map_height(h);
	fill_map_content(map, av);
	if (!map->map[0])
	{
		puts("Error\nEmpty map file\n");
		exit(1);
	}
	map->width = ft_strlen1(map->map[0]);
	validate_map(map);
	return (map);
}

double	deg_to_rad(double angle_degrees)
{
	return (angle_degrees * M_PI / 180.0);
}

void	read_map_lines(t_map *map, int fd)
{
	int		row;
	char	*line;

	row = 0;
	while (row < map->height)
	{
		line = get_next_line(fd);
		if (!line)
		{
			puts("Error\nUnexpected end of file\n");
			close(fd);
			exit(1);
		}
		map->map[row] = line;
		row++;
	}
	map->map[row] = NULL;
}

void	fill_map_content(t_map *map, char *av)
{
	int		fd;
	char	*line;

	fd = open(av, O_RDONLY);
	if (fd == -1)
	{
		puts("Error\nFailed to open map file\n");
		exit(1);
	}
	read_map_lines(map, fd);
	line = get_next_line(fd);
	if (line)
		free(line);
	close(fd);
}
