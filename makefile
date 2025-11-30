NAME      = lo3ba
CC        = cc
CFLAGS    = -Wall -Wextra -Werror -I/usr/local/include
MLX_FLAGS = -lmlx -lXext -lX11 -lm -L/usr/local/lib

SRCS = \
	get_line/get_next_line.c \
	get_line/get_next_line_utils.c \
	parsing/parsing_utils1.c \
	parsing/parsing_utils2.c \
	parsing/colors/parsing.colors.c \
	parsing/colors/parsing.colors2.c \
	parsing/maps/parsing_map1.c \
	parsing/maps/parsing_map2.c \
	parsing/maps/parsing_map3.c \
	parsing/maps/parsing_map4.c \
	parsing/player/parcing_player.c \
	parsing/textures/parsing_textures.c \
	parsing/textures/parsing_textures2.c \
	parsing/textures/parsing_textures3.c \
	src/main.c \
	src/main_utils/main_utils.c \
	src/main_utils/main_utils2.c \
	src/main_utils/main_utils3.c \
	src/main_utils/main_utils4.c \
	src/main_utils/main_utils5.c \
	src/init/init.c \
	src/init/init_utils.c \
	src/movment/movment.c \
	src/movment/movment2.c \
	src/movment/movment3.c \
	src/game_loading/loading_game.c \
	src/game_loading/loading_game2.c \
	src/game_loading/loading_game3.c \
	src/keys/handle_keys.c \
	src/keys/handle_keys2.c \
	src/ray_cast/ray_casting_algo1.c \
	src/ray_cast/ray_casting_algo2.c \
	src/ray_cast/ray_casting_algo3.c \
	src/textures/textures.c \


OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
