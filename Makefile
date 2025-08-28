NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror
MLX = -L/home/$(USER)/Documents/mlx -lmlx -lXext -lX11

SRC = main.c
OBJ = $(SRC:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(MLX) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
