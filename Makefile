NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror
MLX = -L/home/$(USER)/Documents/mlx -lmlx -lXext -lX11

UTILS = $(addprefix src/utils/, ft_malloc.c ft_strcmp.c ft_strjoin.c ft_strdup.c \
		get_next_line.c ft_strlen.c ft_strlcat.c ft_strlcpy.c ft_split.c ft_isspace.c \
		ft_strncmp.c ft_atoi.c ft_perror.c)

PARSE = $(addprefix src/parse/, check_element.c map_check.c parse_file.c)

SRC = src/main.c $(UTILS) $(PARSE)

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
