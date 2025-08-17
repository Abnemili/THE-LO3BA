#include "cub3d.h"


void find_player(t_Map *map, t_Map *player)
{
    int height;
    int whidth;

    height = 0;
    while (height < map->y) // assuming map->y is number of rows
    {
        whidth = 0;
        while (whidth < map->x) // assuming map->x is number of columns
        {
            char cell = map->map[height][whidth];
            if (cell == 'N' || cell == 'S' || cell == 'E' || cell == 'W')
            {
                // Set player's position to the center of the cell
                player->x = whidth + 0.5;
                player->y = height + 0.5;

                // Set direction based on orientation
                if (cell == 'N')
                {
                    player->dir_x = 0;
                    player->dir_y = -1;
                }
                else if (cell == 'S')
                {
                    player->dir_x = 0;
                    player->dir_y = 1;
                }
                else if (cell == 'E')
                {
                    player->dir_x = 1;
                    player->dir_y = 0;
                }
                else if (cell == 'W')
                {
                    player->dir_x = -1;
                    player->dir_y = 0;
                }
                // Optionally, break after finding the player
                return;
            }
            whidth++;
        }
        height++;
    }
}