#ifndef THE_LO3BA_H
# define THE_LO3BA_H

# include <fcntl.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <mlx.h>
# include "../get_line/get_next_line.h"

/* Game settings */
# define TILE 64
# define M_PI 3.14159265358979323846
# define DEG_TO_RAD(angleDegrees) ((angleDegrees) * M_PI / 180.0)

/* Colors */
# define PLAYER_COLOR 0x00FF00    /* Green */
# define COLOR_WALL   0x4B0082    /* Indigo */
# define COLOR_FREE   0x006600    /* Dark Green */

# define MOVE_SPEED    8   // Pixels per keypress
# define PLAYER_SIZE   8   // Size of player square
# define PLAYER_OFFSET 12  // Offset from tile corner

/* Key codes */
# define KEY_ESC 65307
# define KEY_W   119
# define KEY_S   115
# define KEY_A   97
# define KEY_D   100

# define ray_num 1

typedef struct s_ray
{
    double wall_x;      // Wall hit x coordinate
    double wall_y;      // Wall hit y coordinate
    double distance;    // Distance to wall
    int hit_side;       // 0 if horizontal wall, 1 if vertical wall
}   t_ray;

typedef struct s_player
{
    double player_x;    // Player position coordinates
    double player_y;
    double angle;       // Player view direction in degrees
}   t_player;

typedef struct s_map
{
    void    *mlx;
    void    *win;
    void    *img;           // Image buffer
    char    *img_data;      // Image data pointer
    int     img_bpp;
    int     img_size_line;
    int     img_endian;
    char    **map;
    int     width;
    int     height;
    t_ray   rays[ray_num];
    t_player player;
}   t_map;

/* Parsing functions */
t_map   *check_map(char *av);
void    check_ext(char *name);
void    check_shape(t_map *map);
void    check_walls(t_map *map);

/* Image utilities */
void    pixel_put_img(t_map *map, int x, int y, int color);

/* Map initialization */
t_map   *fill_map(char *av);

/* Utility functions */
int     ft_strlen1(char *str);

/* Game loading and rendering */
void    load_game(t_map *map);
void    set_color(t_map *map);
void    draw_player(t_map *map);
void    draw_square(t_map *map, int x, int y, int color);
void    render_scene(t_map *map);
void    setup_rendering(t_map *map);
void    cast_fov_rays(t_map *game);
void    cast_fov_rays_sparse(t_map *game, int ray_spacing);
int     handle_key_input(int keycode, t_map *map);
int     handle_close(t_map *map);

/* Libft functions */
void    ft_putnbr(int n);

/* Ray casting and drawing */
void    draw_pixel(t_map *game, int x, int y, int color);

#endif
