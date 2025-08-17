#ifndef CUB_3D_H
#define CUB_3D_H

# include <fcntl.h>
# include "get_line/get_next_line.h"
# include <mlx.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>



typedef struct s_map
{
    void    *mlx;
    void    *win;
    int     x;
    int      y;
    int     dir_x;
    int     dir_y;
    char    **map;
}t_Map;


t_Map *fill_map(char *av);
void set_color(t_Map *map, int x, int y);
void find_player(t_Map *map, t_Map *player);



#endif