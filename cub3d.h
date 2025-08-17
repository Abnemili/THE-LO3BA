/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 14:04:39 by abnemili          #+#    #+#             */
/*   Updated: 2025/08/17 17:51:32 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_3D_H
# define CUB_3D_H

# include <fcntl.h>
# include "get_line/get_next_line.h"
# include <mlx.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct map
{
    void    *mlx;
    void    *win;
    char    **map;
    int x;
    int y;
    int dir_x;
    int dir_y;
}t_Map;


//filli 2d map 
t_Map *fill_map(char *av);
void set_color(t_Map *map, int  x, int y);
void find_player(t_Map *map, t_Map *player);

#endif