#include "cub3d.h"

#define COLOR_WALL 0XFFFFFF
#define COLOR_FREE 0X000000

// helper function to draw one square of size 32x32
void	draw_square(t_Map *map, int x, int y, int color)
{
	int	dx;
	int	dy;

	dy = 0;
	while (dy < 32)
	{
		dx = 0;
		while (dx < 32)
		{
			mlx_pixel_put(map->mlx, map->win,
				x * 32 + dx, y * 32 + dy, color);
			dx++;
		}
		dy++;
	}
}

void set_color(t_Map *map, int x, int y)
{
    int whidth;
    int height;
    int color;

    height = 0;
    while (height < y)
    {
        whidth = 0;
        while (whidth < x)
        {
            if (map->map[height][whidth] == '1')
                color = COLOR_WALL;
            else if (map->map[height][whidth] == '0')
                color = COLOR_FREE;
            else
            {
                whidth++;
                continue;
            }
            draw_square(map, whidth, height, color);
            whidth++;
        }
        height++;
    }
}

int	ft_strlen1(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	return (i);
}

void	parse_map_file(t_Map *map, char *av)
{
	int		fd;
	int		row;
	char	*line;

	fd = open(av, O_RDONLY);
	if (fd == -1)
		(printf("Error\nOpen failed\n"), exit(1));
	row = 0;
	while (row < map->y)
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

static t_Map	*init_map_height(int h)
{
	t_Map	*map;

	map = malloc(sizeof(t_Map));
	if (!map)
        exit(1);
	map->y = h;
	map->map = malloc(sizeof(char *) * (h + 1));
	if (!map->map)
		(free(map));
	return (map);
}

int	map_height(char *av)
{
	int		fd;
	int		h;
	char	*line;

	fd = open(av, O_RDONLY);
	if (fd == -1)
		(printf("Error\nOpen failed\n"), exit(1));
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

t_Map *fill_map(char *av)
{
    t_Map   *map;
    int     height;
   
   
    height = 0;
    height = map_height(av);
    if (height < 1)
	{
		puts("Error\n the map is invalid");
		exit(1);
	}
    map = init_map_height(height);
    parse_map_file(map, av);
    map->x = ft_strlen1(map->map[0]);
    return map;;
}