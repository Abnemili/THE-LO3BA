/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movment.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:10:06 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/18 19:52:23 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"

void	apply_movement(t_map *map, double *new_x, double *new_y, double buffer)
{
	double		move_x;
	double		move_y;
	t_collision	col;

	calculate_movement(map, map->player.angle, &move_x, &move_y);
	calculate_strafe(map, map->player.angle, &move_x, &move_y);
	col.test_val = *new_x + move_x;
	col.move_val = move_x;
	col.other_val = *new_y;
	check_x_collision(map, new_x, col);
	col.test_val = *new_y + move_y;
	col.move_val = move_y;
	col.other_val = *new_x;
	check_y_collision(map, new_y, col);
	(void)buffer;
}

int	update_player(t_map *map)
{
	double	new_x;
	double	new_y;
	double	angle;

	new_x = map->player.player_x;
	new_y = map->player.player_y;
	angle = map->player.angle;
	update_rotation(map, &angle);
	apply_movement(map, &new_x, &new_y, 10);
	map->player.player_x = new_x;
	map->player.player_y = new_y;
	map->player.angle = angle;
	render_game_view(map);
	return (0);
}

int	handle_key_release(int keycode, t_map *map)
{
	if (keycode == KEY_W)
		map->keys.w = 0;
	else if (keycode == KEY_A)
		map->keys.a = 0;
	else if (keycode == KEY_S)
		map->keys.s = 0;
	else if (keycode == KEY_D)
		map->keys.d = 0;
	else if (keycode == 65361)
		map->keys.left = 0;
	else if (keycode == 65363)
		map->keys.right = 0;
	return (0);
}

int	handle_key_press(int keycode, t_map *map)
{
	if (keycode == KEY_ESC)
		return (handle_close(map));
	if (keycode == KEY_W)
		map->keys.w = 1;
	else if (keycode == KEY_A)
		map->keys.a = 1;
	else if (keycode == KEY_S)
		map->keys.s = 1;
	else if (keycode == KEY_D)
		map->keys.d = 1;
	else if (keycode == 65361)
		map->keys.left = 1;
	else if (keycode == 65363)
		map->keys.right = 1;
	return (0);
}

void	setup_events(t_map *map)
{
	mlx_hook(map->win, 2, 1L << 0, handle_key_press, map);
	mlx_hook(map->win, 3, 1L << 1, handle_key_release, map);
	mlx_hook(map->win, 17, 1L << 17, handle_close, map);
	mlx_loop_hook(map->mlx, update_player, map);
}
