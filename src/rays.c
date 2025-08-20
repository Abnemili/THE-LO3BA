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
#define TILE_SIZE 32

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
    
}
