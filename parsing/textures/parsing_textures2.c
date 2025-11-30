/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_textures2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/18 17:38:19 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	is_valid_xpm_path(const char *path)
{
	int		len;
	int		fd;
	char	buf[1];

	if (!path)
		return (0);
	len = ft_strlen1((char *)path);
	if (len < 5 || ft_strncmp(path + len - 4, ".xpm", 4) != 0)
		return (0);
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (0);
	if (read(fd, buf, 1) <= 0)
	{
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

int	validate_texture(char *texture, char *name, char *dir)
{
	if (!texture)
	{
		printf("Error: Missing %s (%s) texture\n", name, dir);
		return (0);
	}
	if (!is_valid_xpm_path(texture))
	{
		printf("Error: %s texture invalid or not found: %s\n", name, texture);
		return (0);
	}
	return (1);
}

int	validate_all_textures(t_map *map)
{
	int	valid;

	valid = 1;
	if (!validate_texture(map->no_texture, "NO", "North"))
		valid = 0;
	if (!validate_texture(map->so_texture, "SO", "South"))
		valid = 0;
	if (!validate_texture(map->we_texture, "WE", "West"))
		valid = 0;
	if (!validate_texture(map->ea_texture, "EA", "East"))
		valid = 0;
	return (valid);
}

int	parse_no_texture(t_map *map, char *line)
{
	if (map->no_texture)
	{
		printf("Error\nDuplicate NO texture found\n");
		return (0);
	}
	map->no_texture = extract_path(line);
	return (1);
}

int	parse_so_texture(t_map *map, char *line)
{
	if (map->so_texture)
	{
		printf("Error\nDuplicate SO texture found\n");
		return (0);
	}
	map->so_texture = extract_path(line);
	return (1);
}
