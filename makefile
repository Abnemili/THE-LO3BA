# Simple Makefile for cub3d

NAME = cub3d

SRCS = main.c \
	fill_map.c \
	add_player.c \
	get_line/get_next_line.c \
	get_line/get_next_line_utils.c

OBJS = $(SRCS:.c=.o)

CC = gcc
 
CFLAGS = -Wall -Wextra -Werror -g 
MLX_FLAGS = -lmlx -lXext -lX11 -lm


all: $(NAME)


$(NAME): $(OBJS)
	$(CC) $(OBJS) $(MLX_FLAGS) -o $(NAME)

clean:
	rm -f $(OBJS)


fclean: clean
	rm -f $(NAME)

re: fclean all


.PHONY: all clean fclean re