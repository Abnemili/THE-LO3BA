/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 09:44:32 by abnemili          #+#    #+#             */
/*   Updated: 2025/08/23 14:08:34 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"

//calaculate  the map heitgh by reading from the map file .cub
static int	map_height(char *av)
{
	int		fd;
	int		h;
	char	*line;

	fd = open(av, O_RDONLY);
	if (fd == -1)
		(puts("Error\nOpen failed\n"), exit(1)); //forbiden use
	h = 0;
	line = get_next_line(fd);
	while (line && ++h)
	{
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (h);
}

static t_map	*init_map_height(int h)
{
	t_map	*map;

	map = malloc(sizeof(t_map));
	if (!map)
		exit(1);
	map->height = h;
	map->map = malloc(sizeof(char *) * (h + 1));
	if (!map->map)
		free(map);
	return (map);
}


static void	fill_map_content(t_map *map, char *av)
{
	int		fd;
	int		row;
	char	*line;

	fd = open(av, O_RDONLY);
	if (fd == -1)
		(puts("Error\nFaild to open map file \n"), exit(1)); // also forbiden use 
	row = 0;
	while (row < map->height)
	{
		line = get_next_line(fd);
		map->map[row++] = line;
	}
	line = get_next_line(fd);
	if (line)
		free(line);
	close(fd);
	map->map[row] = NULL;
}

// filing the 2d arr frm the mao file 
t_map	*fill_map(char *av)
{
	t_map	*map;
	int		h;

	h = map_height(av);
	if (h < 1)
	{
		puts("Error\n the map is invalid");//3rd forbiden use 
		exit(1);
	}
	map = init_map_height(h);
	fill_map_content(map, av);
	map->width = ft_strlen1(map->map[0]);
	return (map);
}