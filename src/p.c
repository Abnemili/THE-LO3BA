#include "../header/the_lo3ba.h"


// Simplified constants
#define MOVE_SPEED 8  // Pixels per keypress
#define PLAYER_SIZE 8 // Size of player square
#define PLAYER_OFFSET 12 // Offset from tile corner

int is_valid_move(t_map *map, int new_x, int new_y)
{
    int tile_x, tile_y;
    
    // Simple approach: just check the center tile of the player
    tile_x = (new_x + PLAYER_OFFSET + PLAYER_SIZE/2) / TILE;
    tile_y = (new_y + PLAYER_OFFSET + PLAYER_SIZE/2) / TILE;
    
    // Check bounds
    if (tile_x < 0 || tile_x >= map->x ||
        tile_y < 0 || tile_y >= map->y)
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
            int tile_x = (map->player_x + PLAYER_OFFSET + PLAYER_SIZE/2) / TILE + x;
            int tile_y = (map->player_y + PLAYER_OFFSET + PLAYER_SIZE/2) / TILE + y;
            
            if (tile_x >= 0 && tile_x < map->x && tile_y >= 0 && tile_y < map->y)
            {
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
    new_x = map->player_x;
    new_y = map->player_y;
    
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
        map->player_x = new_x;
        map->player_y = new_y;
        
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

void draw_player(t_map *map)
{
    int i, j;
    
    for (i = 0; i < PLAYER_SIZE; i++)
    {
        for (j = 0; j < PLAYER_SIZE; j++)
        {
            mlx_pixel_put(map->mlx, map->win,
                map->player_x + PLAYER_OFFSET + i,
                map->player_y + PLAYER_OFFSET + j,
                PLAYER_COLOR);
        }
    }
}

void draw_square(t_map *map, int x, int y, int color)
{
    int dx, dy;
    
    for (dy = 0; dy < TILE; dy++)
    {
        for (dx = 0; dx < TILE; dx++)
        {
            mlx_pixel_put(map->mlx, map->win,
                x * TILE + dx,
                y * TILE + dy,
                color);
        }
    }
}

void set_color(t_map *map)
{
    int x, y;
    int color;
    
    for (y = 0; y < map->y; y++)
    {
        for (x = 0; x < map->x; x++)
        {
            if (map->map[y][x] == '1')
                color = COLOR_WALL;
            else if (map->map[y][x] == '0')
                color = COLOR_FREE;
            else
                continue;
            draw_square(map, x, y, color);
        }
    }
}

void init_player_position(t_map *map)
{
    int x, y;
    
    // Find first free space ('0') to place player
    for (y = 0; y < map->y; y++)
    {
        for (x = 0; x < map->x; x++)
        {
            if (map->map[y][x] == '0')
            {
                // Place player at a simple position within the tile
                map->player_x = x * TILE + 4;
                map->player_y = y * TILE + 4;
                return;
            }
        }
    }
    
    // Fallback: place at origin
    map->player_x = 4;
    map->player_y = 4;
}

void mlx_connection(t_map *map)
{
    map->mlx = mlx_init();
    if (!map->mlx)
    {
        exit(1);
    }
    map->win = mlx_new_window(map->mlx, map->x * TILE,
        map->y * TILE, "THE_LO3BA");
    if (!map->win)
    {
        mlx_destroy_display(map->mlx);
        free(map->mlx);
        exit(1);
    }
}

void load_game(t_map *map)
{
    mlx_connection(map);
    init_player_position(map);
    
    // Initial render
    set_color(map);
    draw_player(map);
    
    // Set up event hooks
    mlx_hook(map->win, 2, 1, handle_key_input, map);
    mlx_hook(map->win, 17, 0, handle_close, map);
    
    // Start game loop
    mlx_loop(map->mlx);
}

