/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_casting_algo3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 20:05:00 by szaoual           #+#    #+#             */
/*   Updated: 2025/11/29 11:27:25 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"

void	draw_background(t_map *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < SCREEN_HEIGHT)
	{
		x = 0;
		while (x < SCREEN_WIDTH)
		{
			if (y < SCREEN_HEIGHT / 2)
				pixel_put_img(game, x, y, game->ceiling_rgb);
			else
				pixel_put_img(game, x, y, game->floor_rgb);
			x++;
		}
		y++;
	}
}

void	normalize_angle(double *angle)
{
	while (*angle < 0)
		*angle += 360;
	while (*angle >= 360)
		*angle -= 360;
}

void	render_3d_view(t_map *game)
{
	double	fov;
	double	angle_step;
	double	start_angle;
	int		x;

	draw_background(game);
	fov = 60.0;
	angle_step = fov / SCREEN_WIDTH;
	start_angle = game->player.angle - (fov / 2.0);
	x = 0;
	while (x < SCREEN_WIDTH)
	{
		render_single_column(game, x, start_angle, angle_step);
		x++;
	}
}

void	render_game_view(t_map *game)
{
	render_3d_view(game); // on work here
	mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
}
