/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_textures.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/18 17:37:48 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	parse_we_texture(t_map *map, char *line)
{
	if (map->we_texture)
	{
		printf("Error\nDuplicate WE texture found\n");
		return (0);
	}
	map->we_texture = extract_path(line);
	return (1);
}

int	parse_ea_texture(t_map *map, char *line)
{
	if (map->ea_texture)
	{
		printf("Error\nDuplicate EA texture found\n");
		return (0);
	}
	map->ea_texture = extract_path(line);
	return (1);
}

void	cleanup_textures(t_map *map)
{
	if (map->no_texture)
		free(map->no_texture);
	if (map->so_texture)
		free(map->so_texture);
	if (map->we_texture)
		free(map->we_texture);
	if (map->ea_texture)
		free(map->ea_texture);
	if (map->floor_color)
		free(map->floor_color);
	if (map->ceiling_color)
		free(map->ceiling_color);
}

void	cleanup_mlx_images(t_map *map)
{
	if (map->north_tex.img)
		mlx_destroy_image(map->mlx, map->north_tex.img);
	if (map->south_tex.img)
		mlx_destroy_image(map->mlx, map->south_tex.img);
	if (map->west_tex.img)
		mlx_destroy_image(map->mlx, map->west_tex.img);
	if (map->east_tex.img)
		mlx_destroy_image(map->mlx, map->east_tex.img);
	if (map->img)
		mlx_destroy_image(map->mlx, map->img);
}
