/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   the_lo3ba.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abnemili <abnemili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 00:00:00 by szaoual           #+#    #+#             */
/*   Updated: 2025/11/29 08:26:16 by abnemili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

# define TILE 64
# define M_PI 3.14159265358979323846
# define PLAYER_COLOR 0x00FF00
# define COLOR_WALL 0x4B0082
# define COLOR_FREE 0x006600
# define ROTATION_SPEED 3
# define MOVE_SPEED 6
# define PLAYER_SIZE 8
# define PLAYER_OFFSET 12
# define SCREEN_WIDTH 1500
# define SCREEN_HEIGHT 800
# define WALL_HEIGHT_FACTOR 100
# define KEY_ESC 65307
# define KEY_W 119
# define KEY_S 115
# define KEY_A 97
# define KEY_D 100
# define RAY_NUM 320

typedef struct s_ray
{
	double	wall_x;
	double	wall_y;
	double	distance;
	int		hit_side;
}	t_ray;

typedef struct s_player
{
	double	player_x;
	double	player_y;
	double	angle;
}	t_player;

typedef struct s_key_state
{
	int		w;
	int		a;
	int		s;
	int		d;
	int		left;
	int		right;
	int		esc;
}	t_key_state;

typedef struct s_collision
{
	double	test_val;
	double	move_val;
	double	other_val;
}	t_collision;

typedef struct s_dda
{
	double	delta_x;
	double	delta_y;
	double	side_x;
	double	side_y;
	int		step_x;
	int		step_y;
}	t_dda;

typedef struct s_ray_vars
{
	double	ray_x;
	double	ray_y;
	double	ray_dx;
	double	ray_dy;
	int		map_x;
	int		map_y;
}	t_ray_vars;

typedef struct s_texture
{
	void	*img;
	char	*data;
	int		width;
	int		height;
	int		bpp;
	int		size_line;
	int		endian;
}	t_texture;

typedef struct s_wall
{
	int			height;
	int			draw_start;
	int			draw_end;
	int			orig_start;
	t_texture	*texture;
}	t_wall;

typedef struct s_tex_info
{
	int		tex_x;
	double	wall_hit;
	double	step;
	double	pos;
}	t_tex_info;

typedef struct s_map_bounds
{
	int	start_idx;
	int	end_idx;
	int	last_map_line;
	int	found_empty;
}	t_map_bounds;

typedef struct s_map
{
	int			map_fd;
	void		*mlx;
	void		*win;
	void		*img;
	char		*img_data;
	int			img_bpp;
	int			img_size_line;
	int			img_endian;
	char		**map;
	char		*no_texture;
	char		*so_texture;
	char		*we_texture;
	char		*ea_texture;
	char		*floor_color;
	char		*ceiling_color;
	int			floor_rgb;
	int			ceiling_rgb;
	t_texture	north_tex;
	t_texture	south_tex;
	t_texture	west_tex;
	t_texture	east_tex;
	int			width;
	int			height;
	t_ray		rays[RAY_NUM];
	t_player	player;
	int			player_set;
	t_key_state	keys;
}	t_map;

int			handle_key_press(int keycode, t_map *map);
int			handle_key_release(int keycode, t_map *map);
int			update_player(t_map *map);
void		setup_events(t_map *map);
double		deg_to_rad(double angle_degrees);
int			validate_and_set_player(t_map *map);
int			parse_rgb_color(char *color_str);
int			load_textures(t_map *map);
int			get_texture_pixel(t_texture *tex, int x, int y);
int			map_height(char *av);
t_map		*init_map_height(int h);
void		init_map_fields(t_map *map, int h);
void		init_keys(t_map *map);
void		init_rays(t_map *map);
void		fill_map_content(t_map *map, char *av);
void		read_map_lines(t_map *map, int fd);
int			is_valid_move(t_map *map, int new_x, int new_y);
t_map		*fill_map(char *av);
void		pixel_put_img(t_map *map, int x, int y, int color);
int			ft_strlen1(char *str);
int			read_file(t_map *map, char *filename);
int			parse_map_config(t_map *map);
char		*skip_spaces(char *str);
char		*extract_path(char *line);
int			parse_color(char *line);
int			validate_color_format(char *color_str);
int			validate_parsing_completeness(t_map *map);
void		load_game(t_map *map);
void		set_color(t_map *map);
void		draw_player(t_map *map);
void		draw_square(t_map *map, int x, int y, int color);
void		render_scene(t_map *map);
void		setup_rendering(t_map *map);
void		cast_fov_rays(t_map *game);
void		cast_fov_rays_sparse(t_map *game, int ray_spacing);
int			handle_key_input(int keycode, t_map *map);
int			handle_close(t_map *map);
void		ft_putnbr(int n);
void		draw_pixel(t_map *game, int x, int y, int color);
void		render_game_view(t_map *game);
void		free_textures(t_map *map);
void		free_map_array(t_map *map);
void		destroy_mlx_resources(t_map *map);
int			init_mlx_connection(t_map *map);
int			init_rendering(t_map *map);
void		initial_render(t_map *map);
void		setup_player(t_map *map);
int			find_player_position(t_map *map);
int			check_player_position(t_map *map, int x, int y);
void		draw_square_line(t_map *map, int x, int y, int dy);
void		validate_map(t_map *map);
void		update_rotation(t_map *map, double *angle);
void		calculate_movement(t_map *map, double angle, double *mx,
				double *my);
void		calculate_strafe(t_map *map, double angle, double *mx,
				double *my);
int			get_buffer_value(double move, double buffer);
void		apply_movement(t_map *map, double *new_x, double *new_y,
				double buf);
void		check_x_collision(t_map *map, double *new_x, t_collision col);
void		check_y_collision(t_map *map, double *new_y, t_collision col);
int			check_and_open_file(char *filename, t_map *map);
int			count_file_lines(int fd);
char		**allocate_file_content(int line_count, int fd);
int			read_file_lines(char **file_content, int fd);
void		calculate_map_width(t_map *map, char **file_content, int height);
void		init_map_pointers(t_map *map);
void		init_map_keys(t_map *map);
int			find_map_start(t_map *map);
int			validate_map_position(t_map *map, int start_idx);
int			check_map_line(char *s, t_map_bounds *bounds);
int			find_map_bounds(t_map *map, t_map_bounds *bounds);
void		copy_map_lines(char **grid, char **old_map, int start, int lines);
void		free_unused_lines(char **map, int start, int end, int last);
int			extract_map_grid(t_map *map, t_map_bounds *bounds);
int			init_map_struct(t_map **map);
int			process_map_extraction(t_map *map);
int			validate_all(t_map *map, char *filename);
void		init_dda_step(t_dda *dda, t_ray_vars *rv);
int			check_wall_hit(t_map *game, int map_x, int map_y);
void		perform_dda(t_map *game, t_ray_vars *rv, t_dda *dda, int *side);
void		calc_ray_dist(t_ray *ray, t_ray_vars *rv, t_dda *dda, int side);
t_ray		cast_single_ray_3d(t_map *game, double ray_angle);
t_texture	*get_wall_texture(t_map *game, t_ray *ray, double angle);
void		init_wall_params(t_wall *wall, t_ray *ray);
void		init_tex_info(t_tex_info *tex, t_ray *ray, t_wall *wall);
void		draw_wall_column(t_map *game, t_wall *wall, t_tex_info *tex, int x);
void		render_single_column(t_map *game, int x, double start, double step);
void		draw_background(t_map *game);
void		normalize_angle(double *angle);
void		render_3d_view(t_map *game);
void		parse_rgb_components(char *color_str, int *components);
int			load_texture_image(t_map *map, t_texture *tex, char *path);

#endif
