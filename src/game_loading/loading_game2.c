/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading_game2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 18:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/18 19:52:23 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"

int	init_mlx_connection(t_map *map)
{
	map->mlx = mlx_init();
	if (!map->mlx)
	{
		printf("Error: Failed to initialize MLX\n");
		return (0);
	}
	map->win = mlx_new_window(map->mlx, SCREEN_WIDTH,
			SCREEN_HEIGHT, "CUB3D - THE_LO3BA");
	if (!map->win)
	{
		printf("Error: Failed to create window\n");
		mlx_destroy_display(map->mlx);
		free(map->mlx);
		return (0);
	}
	return (1);
}

int	init_rendering(t_map *map)
{
	map->img = mlx_new_image(map->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!map->img)
	{
		printf("Error: Failed to create image buffer\n");
		return (0);
	}
	map->img_data = mlx_get_data_addr(map->img, &map->img_bpp,
			&map->img_size_line, &map->img_endian);
	if (!map->img_data)
	{
		printf("Error: Failed to get image data\n");
		return (0);
	}
	return (1);
}

void	initial_render(t_map *map)
{
	render_game_view(map);
}
