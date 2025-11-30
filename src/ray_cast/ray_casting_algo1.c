/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_casting_algo1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 10:47:23 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/18 20:32:51 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"

void	init_dda_step(t_dda *dda, t_ray_vars *rv)
{
	if (rv->ray_dx < 0)
	{
		dda->step_x = -1;
		dda->side_x = (rv->ray_x / TILE - rv->map_x) * dda->delta_x;
	}
	else
	{
		dda->step_x = 1;
		dda->side_x = (rv->map_x + 1.0 - rv->ray_x / TILE) * dda->delta_x;
	}
	if (rv->ray_dy < 0)
	{
		dda->step_y = -1;
		dda->side_y = (rv->ray_y / TILE - rv->map_y) * dda->delta_y;
	}
	else
	{
		dda->step_y = 1;
		dda->side_y = (rv->map_y + 1.0 - rv->ray_y / TILE) * dda->delta_y;
	}
}

int	check_wall_hit(t_map *game, int map_x, int map_y)
{
	if (map_y < 0 || map_y >= game->height)
		return (1);
	if (map_x < 0 || map_x >= game->width)
		return (1);
	if (!game->map[map_y] || !game->map[map_y][map_x])
		return (1);
	if (game->map[map_y][map_x] == '\n' || game->map[map_y][map_x] == '\r')
		return (1);
	if (game->map[map_y][map_x] == '1' || game->map[map_y][map_x] == ' ')
		return (1);
	return (0);
}

void	perform_dda(t_map *game, t_ray_vars *rv, t_dda *dda, int *side)
{
	int	hit;

	hit = 0;
	while (!hit && rv->map_x >= 0 && rv->map_x < game->width
		&& rv->map_y >= 0 && rv->map_y < game->height)
	{
		if (dda->side_x < dda->side_y)
		{
			dda->side_x += dda->delta_x;
			rv->map_x += dda->step_x;
			*side = 0;
		}
		else
		{
			dda->side_y += dda->delta_y;
			rv->map_y += dda->step_y;
			*side = 1;
		}
		hit = check_wall_hit(game, rv->map_x, rv->map_y);
	}
}

void	calc_ray_dist(t_ray *ray, t_ray_vars *rv, t_dda *dda, int side)
{
	double	raw_distance;

	if (side == 0)
	{
		raw_distance = (rv->map_x - rv->ray_x / TILE
				+ (1 - dda->step_x) / 2.0) / rv->ray_dx;
		ray->wall_x = rv->map_x * TILE;
		ray->wall_y = rv->ray_y + raw_distance * rv->ray_dy * TILE;
	}
	else
	{
		raw_distance = (rv->map_y - rv->ray_y / TILE
				+ (1 - dda->step_y) / 2.0) / rv->ray_dy;
		ray->wall_y = rv->map_y * TILE;
		ray->wall_x = rv->ray_x + raw_distance * rv->ray_dx * TILE;
	}
	ray->distance = raw_distance * TILE;
	ray->hit_side = side;
}

t_ray	cast_single_ray_3d(t_map *game, double ray_angle)
{
	t_ray		ray;
	t_ray_vars	rv;
	t_dda		dda;
	double		ray_angle_rad;
	int			side;

	ray = (t_ray){0};
	rv.ray_x = game->player.player_x + PLAYER_OFFSET + PLAYER_SIZE / 2.0;
	rv.ray_y = game->player.player_y + PLAYER_OFFSET + PLAYER_SIZE / 2.0;
	ray_angle_rad = deg_to_rad(ray_angle);
	rv.ray_dx = cos(ray_angle_rad);
	rv.ray_dy = sin(ray_angle_rad);
	rv.map_x = (int)(rv.ray_x / TILE);
	rv.map_y = (int)(rv.ray_y / TILE);
	dda.delta_x = fabs(1.0 / rv.ray_dx);
	dda.delta_y = fabs(1.0 / rv.ray_dy);
	init_dda_step(&dda, &rv);
	perform_dda(game, &rv, &dda, &side);
	calc_ray_dist(&ray, &rv, &dda, side);
	return (ray);
}
