/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_walls.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 10:05:15 by abnemili          #+#    #+#             */
/*   Updated: 2025/08/18 11:53:11 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"

static void	middle_rows(t_map *map)
{
	int	i;

	i = 1;
	while (i < map->y - 2)
	{
		if (map->map[i][0] != '1' || map->map[i][map->x - 1] != '1')
		{
			puts("Error\nthe map not rounded by walls\n");
			
			exit(1);
		}
		i++;
	}
}

static void	first_row(t_map *map)
{
	int	i;

	i = 0;
	while (i < map->x)
	{
		if (map->map[0][i] != '1')
		{
			puts("Error\nthe map not rounded by walls\n");
		
			exit(1);
		}
		i++;
	}
}

static void	last_row(t_map *map)
{
	int	i;

	i = 0;
	while (i < map->x)
	{
		if (map->map[map->y - 1][i] != '1')
		{
			puts("Error\nthe map not rounded by walls\n");
			
			exit(1);
		}
		i++;
	}
}

void	check_walls(t_map *map)
{
	first_row(map);
	last_row(map);
	middle_rows(map);
}
