#include "../header/the_lo3ba.h"


int is_valid_move(t_map *map, int new_x, int new_y)
{
    int tile_x, tile_y;
    
    /* Convert pixel coordinates to tile coordinates */
    tile_x = new_x / TILE;
    tile_y = new_y / TILE;
    
    /* Check bounds */
    if (tile_x < 0 || tile_x >= map->x || 
        tile_y < 0 || tile_y >= map->y)
        return (0);
    
    /* Check if tile is free (not a wall) */
    if (map->map[tile_y][tile_x] == '1')
        return (0);
    
    return (1);
}

int handle_key_input(int keycode, t_map *map)
{
    int new_x, new_y;
    
    if (keycode == KEY_ESC)
        return (handle_close(map));
    
    /* Calculate new position */
    new_x = map->player_x;
    new_y = map->player_y;
    
    if (keycode == KEY_W)      /* W - Up */
        new_y -= TILE;
    else if (keycode == KEY_S) /* S - Down */
        new_y += TILE;
    else if (keycode == KEY_A) /* A - Left */
        new_x -= TILE;
    else if (keycode == KEY_D) /* D - Right */
        new_x += TILE;
    else
        return (0); /* Unknown key, do nothing */
    
    /* Only move if valid */
    if (is_valid_move(map, new_x, new_y))
    {
        map->player_x = new_x;
        map->player_y = new_y;
        
        /* Redraw everything */
        set_color(map);
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