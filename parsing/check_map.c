/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 09:58:38 by abnemili          #+#    #+#             */
/*   Updated: 2025/08/20 10:58:49 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/the_lo3ba.h"

// here were making sure our map is rectontgular
void	check_shape(t_map *map)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (i < map->height)
	{
		if ((ft_strlen1(map->map[i]) != map->width || ft_strlen1(map->map[map->height
						- 1]) != map->width))
		{
			puts("Error\nthe map not rectangular\n");
			exit(1);
		}
		i++;
	}
}

// cheking extention map if its ".cub"
void	check_ext(char *name)
{
	const char	*ext = ".cub";
	int			len;

	len = strlen(name);
	if (len < 4 || strncmp(name + len - 4, ext, 4) != 0)
	{
		puts("Error\nInvalid map name");
		exit(1);
	}
}