/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 09:44:32 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/18 19:52:23 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"

int	map_height(char *av)
{
	int		fd;
	int		h;
	char	*line;

	fd = open(av, O_RDONLY);
	if (fd == -1)
	{
		puts("Error\nOpen failed\n");
		exit(1);
	}
	h = 0;
	line = get_next_line(fd);
	while (line)
	{
		free(line);
		h++;
		line = get_next_line(fd);
	}
	close(fd);
	return (h);
}

void	init_map_fields(t_map *map, int h)
{
	map->height = h;
	map->width = 0;
	map->mlx = NULL;
	map->win = NULL;
	map->img = NULL;
	map->img_data = NULL;
	map->img_bpp = 0;
	map->img_size_line = 0;
	map->img_endian = 0;
	map->player.player_x = 0;
	map->player.player_y = 0;
	map->player.angle = 0.0;
	map->player_set = 0;
}

void	init_keys(t_map *map)
{
	map->keys.w = 0;
	map->keys.a = 0;
	map->keys.s = 0;
	map->keys.d = 0;
	map->keys.left = 0;
	map->keys.right = 0;
}

void	init_rays(t_map *map)
{
	int	i;

	i = 0;
	while (i < RAY_NUM)
	{
		map->rays[i].wall_x = 0;
		map->rays[i].wall_y = 0;
		map->rays[i].distance = 0;
		map->rays[i].hit_side = 0;
		i++;
	}
}

t_map	*init_map_height(int h)
{
	t_map	*map;

	map = malloc(sizeof(t_map));
	if (!map)
		exit(1);
	init_map_fields(map, h);
	init_keys(map);
	init_rays(map);
	map->map = malloc(sizeof(char *) * (h + 1));
	if (!map->map)
	{
		free(map);
		exit(1);
	}
	return (map);
}
