/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_casting_algo2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 19:52:24 by szaoual           #+#    #+#             */
/*   Updated: 2025/11/18 20:32:51 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"

t_texture	*get_wall_texture(t_map *game, t_ray *ray, double angle)
{
	if (ray->hit_side == 0)
	{
		if (cos(deg_to_rad(angle)) > 0)
			return (&game->east_tex);
		else
			return (&game->west_tex);
	}
	else
	{
		if (sin(deg_to_rad(angle)) > 0)
			return (&game->south_tex);
		else
			return (&game->north_tex);
	}
}

void	init_wall_params(t_wall *wall, t_ray *ray)
{
	wall->height = (int)((TILE * SCREEN_HEIGHT) / ray->distance);
	wall->draw_start = (SCREEN_HEIGHT - wall->height) / 2;
	wall->draw_end = wall->draw_start + wall->height;
	wall->orig_start = wall->draw_start;
	if (wall->draw_start < 0)
		wall->draw_start = 0;
	if (wall->draw_end >= SCREEN_HEIGHT)
		wall->draw_end = SCREEN_HEIGHT - 1;
}

void	init_tex_info(t_tex_info *tex, t_ray *ray, t_wall *wall)
{
	double	wall_hit_pos;

	if (ray->hit_side == 0)
		wall_hit_pos = ray->wall_y;
	else
		wall_hit_pos = ray->wall_x;
	tex->tex_x = (int)(wall_hit_pos) % TILE;
	if (tex->tex_x < 0)
		tex->tex_x += TILE;
	tex->tex_x = (tex->tex_x * wall->texture->width) / TILE;
	tex->step = (double)wall->texture->height / wall->height;
	tex->pos = (wall->draw_start - wall->orig_start) * tex->step;
}

void	draw_wall_column(t_map *game, t_wall *wall, t_tex_info *tex, int x)
{
	int	y;
	int	tex_y;
	int	color;

	y = wall->draw_start;
	while (y <= wall->draw_end)
	{
		tex_y = (int)tex->pos;
		if (tex_y >= wall->texture->height)
			tex_y = wall->texture->height - 1;
		if (tex_y < 0)
			tex_y = 0;
		color = get_texture_pixel(wall->texture, tex->tex_x, tex_y);
		pixel_put_img(game, x, y, color);
		tex->pos += tex->step;
		y++;
	}
}

void	render_single_column(t_map *game, int x, double start, double step)
{
	double		angle;
	double		angle_diff;
	t_ray		ray;
	t_wall		wall;
	t_tex_info	tex;

	angle = start + (x * step);
	normalize_angle(&angle);
	ray = cast_single_ray_3d(game, angle);
	angle_diff = angle - game->player.angle;
	while (angle_diff < -180)
		angle_diff += 360;
	while (angle_diff > 180)
		angle_diff -= 360;
	ray.distance = ray.distance * cos(deg_to_rad(angle_diff));
	if (ray.distance > 0)
	{
		init_wall_params(&wall, &ray);
		wall.texture = get_wall_texture(game, &ray, angle);
		init_tex_info(&tex, &ray, &wall);
		draw_wall_column(game, &wall, &tex, x);
	}
}
