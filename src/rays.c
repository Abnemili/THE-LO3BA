#include "../header/the_lo3ba.h"

///in term of casting rays 
// first its gonna be like this 
// to do
// translate from degree to radian
// is the corinat is out of bounnd--> is ft_bounds
// cast a ray
// cast all rays of fov


#include "../header/the_lo3ba.h"

#define SCREEN_WIDTH 800
#define SCRENN_HEIGHT 600
#define FOV 60.0
#define PI 3.14159265359


// convert degree to radian
double deg_to_rad(double degree)
{
    return (degree * PI / 180.0);
}

// check if the or the player position player is oout of the map
int ft_bounds(t_map *map, int   width, int height)
{
    //check for bounderies 
    if (width < 0 || width >= map->width ||
        height < 0 || height >= map->height)
         return (1);
    return (map->map[height][width] == '1');
}
//this function cast a singl ray 

double cast_ray(t_map *map, double ray_angle)
{
    double  ray_x;
    double  ray_y;
    double  rad_angle;

    ray_x = map->player->player_x; // the player position x
    ray_y = map->player->player_y; // the player position y
    rad_angle = deg_to_rad(ray_angle);

    double  ray_dx = cos(rad_angle);
    double  ray_dy = sin(rad_angle);


    double  step_size = 0.1;

    while (1)
    {
        // move ray forward 
        ray_x = ray_dx * step_size;
        ray_y = ray_dy * step_size;

        int map_x = (int)(ray_x / TILE);
        int map_y = (int)(ray_y / TILE);


        /// getting the lenght of ray casted form the player to the hitting wall
        if (ft_bounds(map, map_x, map_y))
        {
            double distance = sqrt(
                (ray_x - map->player->player_x) * (ray_x - map->player->player_x)
                + (ray_y - map->player->player_y) *(ray_y - map->player->player_y)
            );
            return (distance);
        }
    

        // safly cheching the ray lenght doesnt goo to fare 
        if (sqrt(
                (ray_x - map->player->player_x) * (ray_x - map->player->player_x)
                + (ray_y - map->player->player_y) *(ray_y - map->player->player_y)
            ) > 1000)
            {
                return 1000;
            }
    }
}
