NAME      = lo3ba
CC        = cc
CFLAGS    = -Wall -Wextra -Werror -fsanitize=address
MLX_FLAGS = -lmlx -lXext -lX11

SRCS = \
	src/main.c \
	src/init.c \
	src/loading_game.c \
	parsing/utils.c \
	parsing/parsing.c \
	parsing/check_map.c \
	parsing/check_walls.c \
	parsing/mini_libft.c \
	get_line/get_next_line.c \
	get_line/get_next_line_utils.c \
	pp.c 
# 	src/handle_keys.c \
# 	src/draw_rays.c \

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
