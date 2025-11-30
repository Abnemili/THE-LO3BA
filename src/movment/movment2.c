/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movment2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 18:10:00 by szaoual           #+#    #+#             */
/*   Updated: 2025/11/18 19:52:23 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"

void	update_rotation(t_map *map, double *angle)
{
	if (map->keys.left)
	{
		*angle -= ROTATION_SPEED;
		if (*angle < 0)
			*angle += 360;
	}
	if (map->keys.right)
	{
		*angle += ROTATION_SPEED;
		if (*angle >= 360)
			*angle -= 360;
	}
}

void	calculate_movement(t_map *map, double angle, double *mx, double *my)
{
	*mx = 0;
	*my = 0;
	if (map->keys.w)
	{
		*mx += cos(deg_to_rad(angle)) * MOVE_SPEED;
		*my += sin(deg_to_rad(angle)) * MOVE_SPEED;
	}
	if (map->keys.s)
	{
		*mx -= cos(deg_to_rad(angle)) * MOVE_SPEED;
		*my -= sin(deg_to_rad(angle)) * MOVE_SPEED;
	}
}

void	calculate_strafe(t_map *map, double angle, double *mx, double *my)
{
	if (map->keys.a)
	{
		*mx += cos(deg_to_rad(angle - 90)) * MOVE_SPEED;
		*my += sin(deg_to_rad(angle - 90)) * MOVE_SPEED;
	}
	if (map->keys.d)
	{
		*mx += cos(deg_to_rad(angle + 90)) * MOVE_SPEED;
		*my += sin(deg_to_rad(angle + 90)) * MOVE_SPEED;
	}
}

int	get_buffer_value(double move, double buffer)
{
	if (move > 0)
		return ((int)buffer);
	return ((int)-buffer);
}

void	check_x_collision(t_map *map, double *new_x, t_collision col)
{
	int	x_check;

	x_check = (int)(col.test_val + get_buffer_value(col.move_val, 10));
	if (is_valid_move(map, x_check, (int)col.other_val))
		*new_x = col.test_val;
}
