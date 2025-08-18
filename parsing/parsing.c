/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 09:33:17 by abnemili          #+#    #+#             */
/*   Updated: 2025/08/18 11:53:03 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"

t_map	*check_map(char *av)
{
	t_map	*map;

	check_ext(av);      // done -> checking for argument and ext map .cub
	map = fill_map(av);  // done -> filling the map with content from the map fil into our 2d arr
	if (map->x > 1920 || map->y > 1080) // settign limit for the resoulition
	{
		puts("Error\nthe map is too big!!\n"); //forbiden use
		exit(1);
	}
	check_shape(map);
	check_walls(map);
	// check_src(map); this functoin check the map resources later 
	// check_path(av, map); this one i think i wouldnt need it 
	return (map);
}