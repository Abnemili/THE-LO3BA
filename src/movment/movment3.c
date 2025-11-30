/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movment3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 18:15:00 by szaoual           #+#    #+#             */
/*   Updated: 2025/11/18 19:52:23 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/the_lo3ba.h"

void	check_y_collision(t_map *map, double *new_y, t_collision col)
{
	int	y_check;

	y_check = (int)(col.test_val + get_buffer_value(col.move_val, 10));
	if (is_valid_move(map, (int)col.other_val, y_check))
		*new_y = col.test_val;
}
