/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szaoual <szaoual@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by abnemili          #+#    #+#             */
/*   Updated: 2025/11/18 17:35:12 by szaoual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../header/the_lo3ba.h"

/* Structures */
typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

typedef struct s_flood_data
{
	t_point	*stack;
	char	**visited;
	int		*stack_size;
}	t_flood_data;

/* Utils functions */
int		ft_strlen1(char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*skip_spaces(char *str);
char	*skip_to_comma(char *ptr);
char	*copy_string(char *src, int len);
char	*trim_end_spaces(char *start, char *end);
char	*extract_path(char *line);
int		parse_config_line(t_map *map, char *line);
int		validate_parsing_completeness(t_map *map);
void	free_visited(char **visited, t_map *map);

/* Color functions */
int		check_color_chars(char *color_str, int *commas, int *num_count);
char	*parse_rgb_value(char *ptr, int *value, int *has_digits);
int		validate_rgb_component(char **ptr);
int		check_basic_format(char *color_str, int *commas, int *num_count);
int		validate_color_format(char *color_str);
int		validate_color(char *color, char *name, char *type);
int		validate_all_colors(t_map *map);
int		parse_ceiling_color(t_map *map, char *line);
int		parse_floor_color(t_map *map, char *line);

/* Texture functions */
int		is_valid_xpm_path(const char *path);
int		validate_texture(char *texture, char *name, char *dir);
int		validate_all_textures(t_map *map);
int		parse_no_texture(t_map *map, char *line);
int		parse_so_texture(t_map *map, char *line);
int		parse_we_texture(t_map *map, char *line);
int		parse_ea_texture(t_map *map, char *line);
void	cleanup_textures(t_map *map);
void	cleanup_mlx_images(t_map *map);
void	cleanup_mlx(t_map *map);
void	cleanup_map(t_map *map);

/* Map functions */
int		parse_map_config(t_map *map);
int		check_first_row(t_map *map);
int		check_last_row(t_map *map);
int		get_row_length(char *row);
int		find_first_nonspace(char *row, int len);
int		find_last_nonspace(char *row, int len);
int		check_row_borders(t_map *map, int y, int first_char, int last_char);
int		should_skip_row(t_map *map, int y, int *len);
int		validate_row_borders(t_map *map, int y, int len);
int		check_middle_rows(t_map *map);
int		check_map_borders(t_map *map);
void	process_map_char(t_map *map, int x, int y, int *player_count);
void	scan_map_for_player(t_map *map, int *pc, int *px, int *py);
int		validate_and_set_player(t_map *map);
char	get_map_char(t_map *map, int x, int y);
int		check_boundary_escape(t_map *map, int nx, int ny);
int		check_map_closed(t_map *map, int start_x, int start_y);
void	cleanup_map_array(t_map *map);

/* Flood fill functions */
int		flood_fill_loop(t_map *map, t_flood_data *data);
void	add_neighbor_to_stack(t_flood_data *data, t_point neighbor);
int		process_neighbors(t_map *map, t_point p, t_flood_data *data);
char	**allocate_visited(t_map *map);
t_point	*allocate_stack(t_map *map, char **visited);

/* Player functions */
void	set_player_angle(t_map *map, char c);
void	handle_player_spawn(t_map *map, int x, int y, int *player_count);
void	init_directions(int *dx, int *dy);

#endif
