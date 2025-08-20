
#ifndef THE_LO3BA_H  
# define THE_LO3BA_H    

# include <fcntl.h>
# include "../get_line/get_next_line.h"
# include <mlx.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_player{
    double player_x;            // x and y are the player position cordinate 
    double player_y;
    double angle;           //player view direction in degree
}t_player;


typedef struct s_map{
    void    *mlx;
    void    *win;
    char    **map;
    int     width;
    int     height;
    t_player *player;
}t_map;



/* Game settings */
#define TILE 32

/* Colors */
#define PLAYER_COLOR 0x00FF00    /* Green */
#define COLOR_WALL   0X8B0000    /* White */
#define COLOR_FREE   0x000000    /* Black */

#define MOVE_SPEED 8  // Pixels per keypress
#define PLAYER_SIZE 8 // Size of player square
#define PLAYER_OFFSET 12 // Offset from tile corner

/* Key codes */
#define KEY_ESC 65307
#define KEY_W   119
#define KEY_S   115
#define KEY_A   97
#define KEY_D   100


// the parsing functions 
t_map	*check_map(char *av);
void	check_ext(char *name);
void	check_shape(t_map *map);
void	check_walls(t_map *map);

//init the dara for the map 
t_map	*fill_map(char *av);

// some utils shit
int	ft_strlen1(char *str);



//loading the game and stuff
void	load_game(t_map *map);
void    set_color(t_map *map);
int     handle_key_input(int keycode, t_map *map);
void    draw_player(t_map *map);
int     handle_close(t_map *map);
void draw_square(t_map *map, int x, int y, int color);

//the libft functions
void	ft_putnbr(int n);

#endif