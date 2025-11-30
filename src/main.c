/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 09:28:38 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/18 20:32:51 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"
#include "../parsing/parsing.h"

int	check_and_open_file(char *filename, t_map *map)
{
	int	len;

	if (!filename)
		return (0);
	len = ft_strlen1(filename);
	if (len < 5)
		return (0);
	if (ft_strncmp(filename + len - 4, ".cub", 4) != 0)
		return (0);
	if (filename[0] == '.' && len == 4)
		return (0);
	map->map_fd = open(filename, O_RDONLY);
	if (map->map_fd < 0)
		return (0);
	return (1);
}

int	read_file(t_map *map, char *filename)
{
	char	**file_content;
	int		line_count;
	int		i;

	if (!map || !filename || map->map_fd < 0)
		return (0);
	line_count = count_file_lines(map->map_fd);
	close(map->map_fd);
	if (line_count == 0)
		return (0);
	map->map_fd = open(filename, O_RDONLY);
	if (map->map_fd < 0)
		return (0);
	file_content = allocate_file_content(line_count, map->map_fd);
	if (!file_content)
		return (0);
	i = read_file_lines(file_content, map->map_fd);
	close(map->map_fd);
	map->map = file_content;
	map->height = i;
	calculate_map_width(map, file_content, i);
	return (1);
}

int	main(int ac, char **av)
{
	t_map	*map;

	if (ac != 2)
	{
		printf("Error\nUsage: ./cub3d <map.cub>\n");
		return (1);
	}
	if (!init_map_struct(&map))
		return (1);
	if (!validate_all(map, av[1]))
	{
		cleanup_map(map);
		return (1);
	}
	load_game(map);
	cleanup_map(map);
	return (0);
}
