#include "../header/the_lo3ba.h"


typedef struct s_player{
    double player_x;            // x and y are the player position cordinate 
    double player_y;
    double angle;           //player view direction in degree
}t_player;


typedef struct s_map {
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

// Initialize image buffer after mlx_connection
void init_image(t_map *map)
{
    map->img = mlx_new_image(map->mlx, map->width * TILE, map->height * TILE);
    map->img_data = mlx_get_data_addr(map->img, &map->img_bpp, &map->img_size_line, &map->img_endian);
}

// Draw pixel onto image buffer instead of directly to window
void pixel_put_img(t_map *map, int x, int y, int color)
{
    int index;

    index = y * map->img_size_line + x * (map->img_bpp / 8);
    *(unsigned int *)(map->img_data + index) = color;
}

// Clear the entire image buffer
void clear_image(t_map *map)
{
    int x, y;
    for (y = 0; y < map->height * TILE; y++)
        for (x = 0; x < map->width * TILE; x++)
            pixel_put_img(map, x, y, 0x000000); // black background
}

// Render the entire scene into the buffer
void render_scene(t_map *map)
{
    clear_image(map);            // Fill background
    set_color(map);              // Draw static map (walls & free spaces)
    draw_player(map);            // Draw dynamic player
    mlx_put_image_to_window(map->mlx, map->win, map->img, 0, 0); // Show buffer
}

void setup_rendering(t_map *map)
{
    init_image(map);
}
// drawing the player 
void draw_player(t_map *map)
{
    int i;
    int j;

    i = 0;
    while (PLAYER_SIZE > i)
    {
        j = 0;
        while (PLAYER_SIZE > j)
        {
            mlx_pixel_put(map->mlx, map->win, 
                map->player->player_x  + PLAYER_OFFSET + i,
                map->player->player_y + PLAYER_OFFSET + j, 
                PLAYER_COLOR);
            j++;
        }
        i++;
    }
}

// seting the color in the actual block or tiles
void draw_square(t_map *map, int x, int y, int color)
{
    int dx;
    int dy;

    dy = 0;
    while (TILE > dy)
    {
        dx = 0;
        while (TILE > dx)
        {
            mlx_pixel_put(map->mlx, map->win,
                x * TILE + dx, // here
                y * TILE + dy, 
                color);
            dx++;
        }
        dy++;
    }
}

// colorint the free space and the walls
void set_color(t_map *map)
{
    int x;
    int y;
    int color;

    y = 0;
    while (map->height > y)
    {
        x = 0;
        while (map->width > x)
        {
            if(map->map[y][x] == 48)
                color = COLOR_FREE;
            else if (map->map[y][x] == 49)
                color = COLOR_WALL;
            else 
                continue;
            draw_square(map, x , y, color);
            x++;
        }
        y++;
    }
}


// init the player first position in the map
void init_player_position(t_map *map)
{
    int x;
    int y;

    y = 0;
   
    while (map->height > y)
    {
        x = 0;
        while (map->width > x)
        {
            if (map->map[y][x] == '0')
            {
                map->player->player_x = x * TILE + 4;
                map->player->player_y = y * TILE + 4;
                return;
            }
            x++;
        }
        y++;
    }
    map->player->player_x = 4;
    map->player->player_y = 4;
}


void mlx_connection(t_map *map)
{
    map->mlx = mlx_init();
    if (!map->mlx)
    {
        exit(1);
    }
    map->win = mlx_new_window(map->mlx, map->width * TILE,
        map->height * TILE, "THE_LO3BA");
    if (!map->win)
    {
        mlx_destroy_display(map->mlx);
        free(map->mlx);
        exit(1);
    }
}

void load_game(t_map *map)
{
    mlx_connection(map); // done

    init_player_position(map); // done
    
    // Initial render
    set_color(map);  // done
    draw_player(map); // done
    draw_ray(map); // a segfault happeing here 
    
    // Set up event hooks
    mlx_hook(map->win, 2, 1, handle_key_input, map);
    mlx_hook(map->win, 17, 0, handle_close, map);
    
    // Start game loop
    mlx_loop(map->mlx);
}


//calaculate  the map heitgh by reading from the map file .cub
static int	map_height(char *av)
{
	int		fd;
	int		h;
	char	*line;

	fd = open(av, O_RDONLY);
	if (fd == -1)
		(puts("Error\nOpen failed\n"), exit(1)); //forbiden use
	h = 0;
	line = get_next_line(fd);
	while (line && ++h)
	{
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (h);
}

static t_map	*init_map_height(int h)
{
	t_map	*map;

	map = malloc(sizeof(t_map));
	if (!map)
		exit(1);
	map->height = h;
	map->map = malloc(sizeof(char *) * (h + 1));
	if (!map->map)
		free(map);
	map->player = malloc(sizeof(t_player));
    if (!map->player)
    {
        free(map->player);
        free(map);
        exit(1);
    }
	return (map);
}


static void	fill_map_content(t_map *map, char *av)
{
	int		fd;
	int		row;
	char	*line;

	fd = open(av, O_RDONLY);
	if (fd == -1)
		(puts("Error\nFaild to open map file \n"), exit(1)); // also forbiden use 
	row = 0;
	while (row < map->height)
	{
		line = get_next_line(fd);
		map->map[row++] = line;
	}
	line = get_next_line(fd);
	if (line)
		free(line);
	close(fd);
	map->map[row] = NULL;
}

// filing the 2d arr frm the mao file 
t_map	*fill_map(char *av)
{
	t_map	*map;
	int		h;

	h = map_height(av);
	if (h < 1)
	{
		puts("Error\n the map is invalid");//3rd forbiden use 
		exit(1);
	}
	map = init_map_height(h);
	fill_map_content(map, av);
	map->width = ft_strlen1(map->map[0]);
	return (map);
}


int is_valid_move(t_map *map, int new_x, int new_y)
{
    int tile_x, tile_y;
    
    // Simple approach: just check the center tile of the player
    tile_x = (new_x + PLAYER_OFFSET + PLAYER_SIZE/2) / TILE;
    tile_y = (new_y + PLAYER_OFFSET + PLAYER_SIZE/2) / TILE;
    
    // Check bounds
    if (tile_x < 0 || tile_x >= map->width ||
        tile_y < 0 || tile_y >= map->height)
        return (0);
    
    // Check if center tile is free (not a wall)
    if (map->map[tile_y][tile_x] == '1')
        return (0);
    
    return (1);
}

void clear_player_area(t_map *map)
{
    int x, y;
    
    // Simply redraw a small area around the player
    for (y = -1; y <= 1; y++)
    {
        for (x = -1; x <= 1; x++)
        {
            int tile_x = (map->player->player_x + PLAYER_OFFSET + PLAYER_SIZE/2) / TILE + x;
            int tile_y = (map->player->player_y + PLAYER_OFFSET + PLAYER_SIZE/2) / TILE + y;
            
            if (tile_x >= 0 && tile_x < map->width && tile_y >= 0 && tile_y < map->height)
            {//d
                int color;
                if (map->map[tile_y][tile_x] == '1')
                    color = COLOR_WALL;
                else if (map->map[tile_y][tile_x] == '0')
                    color = COLOR_FREE;
                else
                    continue;
                draw_square(map, tile_x, tile_y, color);
            }
        }
    }
}

int handle_key_input(int keycode, t_map *map)
{
    int new_x, new_y;
    
    if (keycode == KEY_ESC)
        return (handle_close(map));
    
    // Calculate new position
    new_x = map->player->player_x;
    new_y = map->player->player_y;
    
    if (keycode == KEY_W) // W - Up
        new_y -= MOVE_SPEED;
    else if (keycode == KEY_S) // S - Down
        new_y += MOVE_SPEED;
    else if (keycode == KEY_A) // A - Left
        new_x -= MOVE_SPEED;
    else if (keycode == KEY_D) // D - Right
        new_x += MOVE_SPEED;
    else
        return (0); // Unknown key, do nothing
    
    // Check if move is valid
    if (is_valid_move(map, new_x, new_y))
    {
        // Clear previous player position
        clear_player_area(map);
        
        // Update position
        map->player->player_x = new_x;
        map->player->player_y = new_y;
        
        // Draw player at new position
        draw_player(map);
    }
    
    return (0);
}

int handle_close(t_map *map)
{
    if (map->win)
        mlx_destroy_window(map->mlx, map->win);
    if (map->mlx)
    {
        mlx_destroy_display(map->mlx);
        free(map->mlx);
    }
    exit(0);
    return (0);
}
