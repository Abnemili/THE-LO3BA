#include <math.h>
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FOV 60.0  // Field of view in degrees
#define PI 3.14159265359
#define TILE_SIZE 32


typedef struct {
    double x, y;        // Player position
    double angle;       // Player viewing direction in degrees
} t_Player;

typedef struct {
    void *mlx;
    void *win;
    char **map;
    int map_width;
    int map_height;
    t_Player player;
} t_Game;

// Convert degrees to radians
double deg_to_rad(double degrees)
{
    return degrees * PI / 180.0;
}

//ge ting the angle from degree to radian

// Check if a position hits a wall
int is_wall(t_Game *game, int map_x, int map_y)
{
    // Check bounds first
    if (map_x < 0 || map_x >= game->map_width || 
        map_y < 0 || map_y >= game->map_height)
        return 1; // Treat out of bounds as walls
    
    return (game->map[map_y][map_x] == '1');
}
double cast_ray(t_Game *game, double ray_angle)
{
    double ray_x = game->player.x;
    double ray_y = game->player.y;
    
    // Convert angle to radians
    double rad_angle = deg_to_rad(ray_angle);
    
    // Calculate ray direction (how much x,y changes per step)
    double ray_dx = cos(rad_angle);
    double ray_dy = sin(rad_angle);
    
    // Step size - smaller = more accurate but slower
    double step_size = 0.1;
    
    // Move ray step by step until it hits a wall
    while (1)
    {
        // Move ray forward
        ray_x += ray_dx * step_size;
        ray_y += ray_dy * step_size;
        
        // Convert to map coordinates
        int map_x = (int)(ray_x / TILE_SIZE);
        int map_y = (int)(ray_y / TILE_SIZE);
        
        // Check if we hit a wall
        if (is_wall(game, map_x, map_y))
        {
            // Calculate distance from player to wall
            double distance = sqrt(
                (ray_x - game->player.x) * (ray_x - game->player.x) + 
                (ray_y - game->player.y) * (ray_y - game->player.y)
            );
            return distance;
        }
        
        // Safety check - don't let ray go too far
        if (sqrt((ray_x - game->player.x) * (ray_x - game->player.x) + 
                (ray_y - game->player.y) * (ray_y - game->player.y)) > 1000)
        {
            return 1000; // Max distance
        }
    }
}

// Cast rays for the entire screen
void cast_all_rays(t_Game *game)
{
    printf("=== RAY CASTING DEBUG ===\n");
    printf("Player position: (%.2f, %.2f)\n", game->player.x, game->player.y);
    printf("Player angle: %.2f degrees\n", game->player.angle);
    
    // Calculate angle step between rays
    double angle_step = FOV / SCREEN_WIDTH;
    double start_angle = game->player.angle - (FOV / 2.0);
    
    printf("FOV: %.1f degrees, Angle step: %.4f degrees\n", FOV, angle_step);
    printf("Casting rays from %.2f to %.2f degrees\n", 
           start_angle, start_angle + FOV);
    
    // Cast rays for each screen column
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        // Calculate current ray angle
        double ray_angle = start_angle + (x * angle_step);
        
        // Cast the ray
        double distance = cast_ray(game, ray_angle);
        
        // Debug: Print info for some rays
        if (x % 100 == 0) // Every 100th ray
        {
            printf("Ray %d: angle=%.2f°, distance=%.2f\n", 
                   x, ray_angle, distance);
        }
        
        // TODO: Use this distance to draw walls in Phase 3
        // For now, we just calculate and store/print the distance
    }
    printf("========================\n\n");
}

// Example usage function
void test_raycasting()
{
    t_Game game;
    
    // Setup a simple test map
    char *test_map[] = {
        "111111111",
        "100000001", 
        "100010001",
        "100000001",
        "100000001",
        "111111111"
    };
    
    game.map = test_map;
    game.map_width = 9;
    game.map_height = 6;
    
    // Place player in center, facing north (up)
    game.player.x = 4.5 * TILE_SIZE; // Center of map
    game.player.y = 3.0 * TILE_SIZE;
    game.player.angle = -90.0; // -90° = North (up)
    
    // Cast rays
    cast_all_rays(&game);
    
    // Test different player angles
    printf("\n--- Testing different angles ---\n");
    game.player.angle = 0.0;   // East
    printf("Facing East (0°):\n");
    cast_all_rays(&game);
    
    game.player.angle = 90.0;  // South  
    printf("Facing South (90°):\n");
    cast_all_rays(&game);
}