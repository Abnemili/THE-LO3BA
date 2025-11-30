/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 20:10:00 by szaoual           #+#    #+#             */
/*   Updated: 2025/11/19 20:38:08 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"

void	parse_rgb_components(char *color_str, int *components)
{
	int	i;
	int	num;
	int	comp_idx;

	i = 0;
	comp_idx = 0;
	while (comp_idx < 3)
	{
		while (color_str[i] == ' ')
			i++;
		num = 0;
		while (color_str[i] >= '0' && color_str[i] <= '9')
		{
			num = num * 10 + (color_str[i] - '0');
			i++;
		}
		if (num > 255)
			num = 255;
		components[comp_idx] = num;
		comp_idx++;
		while (color_str[i] == ' ' || color_str[i] == ',')
			i++;
		if (color_str[i] == '\0')
			break ;
	}
}

int	parse_rgb_color(char *color_str)
{
	int	components[3];
	int	r;
	int	g;
	int	b;

	if (!color_str)
		return (0);
	parse_rgb_components(color_str, components);
	r = components[0];
	g = components[1];
	b = components[2];
	return ((r << 16) | (g << 8) | b);
}

int	load_texture_image(t_map *map, t_texture *tex, char *path)
{
	tex->img = mlx_xpm_file_to_image(map->mlx, path,
			&tex->width, &tex->height);
	if (!tex->img)
	{
		printf("Error\nFailed to load texture: %s\n", path);
		return (0);
	}
	tex->data = mlx_get_data_addr(tex->img, &tex->bpp,
			&tex->size_line, &tex->endian);
	return (1);
}

int	load_textures(t_map *map)
{
	if (!load_texture_image(map, &map->north_tex, map->no_texture))
		return (0);
	if (!load_texture_image(map, &map->south_tex, map->so_texture))
		return (0);
	if (!load_texture_image(map, &map->west_tex, map->we_texture))
		return (0);
	if (!load_texture_image(map, &map->east_tex, map->ea_texture))
		return (0);
	return (1);
}

int	get_texture_pixel(t_texture *tex, int x, int y)
{
	int	pixel_index;

	if (x < 0 || x >= tex->width || y < 0 || y >= tex->height)
		return (0);
	pixel_index = (y * tex->size_line) + (x * (tex->bpp / 8));
	return (*(int *)(tex->data + pixel_index));
}
