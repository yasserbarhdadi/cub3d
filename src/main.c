/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 14:59:43 by yabarhda          #+#    #+#             */
/*   Updated: 2025/12/26 06:57:15 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/main.h"

int	error_check(int ac, char **av)
{
	int	fd;
	int	len;

	if (ac != 2)
		return (printf("Error\nNot enough arguments\n"), 0);
	fd = open(av[1], O_RDONLY);
	if (fd == -1)
		return (printf("Error\nNo such file or directory\n"), 0);
	close(fd);
	len = ft_strlen(av[1]);
	if (ft_strcmp(".cub", &av[1][len - 4]))
		return (printf("Error\nInvalid file format\n"), 0);
	return (1);
}

static void	init_data(t_data *data)
{
	data->mlx = NULL;
	data->map = NULL;
	data->img = NULL;
	data->img_addr = NULL;
	data->size_line = 0;
	data->bpp = 0;
	data->endien = 0;
	data->width = WIDTH;
	data->height = HEIGHT;
	data->map_array = NULL;
	data->map_width = 0;
	data->map_height = 0;
	data->texture = ft_malloc(sizeof(t_texture));
	data->texture->north = NULL;
	data->texture->south = NULL;
	data->texture->west = NULL;
	data->texture->east = NULL;
	data->texture->floor = NULL;
	data->texture->ceiling = NULL;
	data->player.x = 0;
	data->player.y = 0;
	data->player.angle = 0;
	data->player.direction = 0;
	data->player.key_w = false;
	data->player.key_a = false;
	data->player.key_s = false;
	data->player.key_d = false;
	data->player.key_right = false;
	data->player.key_left = false;
}

int	clean_exit(t_data *data)
{
	mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	ft_malloc(-42);
	exit(0);
}

void	init_player(t_data *data)
{
	if (data->player.direction == 'N')
		data->player.angle = 0.0;
	else if (data->player.direction == 'E')
		data->player.angle = PI / 2.0;
	else if (data->player.direction == 'S')
		data->player.angle = PI;
	else if (data->player.direction == 'W')
		data->player.angle = 3.0 * PI / 2.0;
}

int	on_keypress(int key, t_data *data)
{
	if (key == KEY_ESC)
		clean_exit(data);
	if (key == KEY_W)
		data->player.key_w = true;
	if (key == KEY_A)
		data->player.key_a = true;
	if (key == KEY_S)
		data->player.key_s = true;
	if (key == KEY_D)
		data->player.key_d = true;
	if (key == KEY_RIGHT)
		data->player.key_right = true;
	if (key == KEY_LEFT)
		data->player.key_left = true;
	return (0);
}

int	on_keyrelease(int key, t_data *data)
{
	if (key == KEY_W)
		data->player.key_w = false;
	if (key == KEY_A)
		data->player.key_a = false;
	if (key == KEY_S)
		data->player.key_s = false;
	if (key == KEY_D)
		data->player.key_d = false;
	if (key == KEY_RIGHT)
		data->player.key_right = false;
	if (key == KEY_LEFT)
		data->player.key_left = false;	
	return (0);
}

void	move_player(t_data *data)
{
	if (data->player.key_w)
		printf("Key W is pressed\n");
	if (data->player.key_a)
		printf("Key A is pressed\n");
	if (data->player.key_s)
		printf("Key S is pressed\n");
	if (data->player.key_d)
		printf("Key D is pressed\n");
	if (data->player.key_right)
		printf("Key right is pressed\n");
	if (data->player.key_left)
		printf("Key left is pressed\n");
}

int	on_gameupdate(t_data *data)
{
	move_player(data);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	*data;

	if (!error_check(ac, av))
		return (1);
	data = ft_malloc(sizeof(t_data));
	init_data(data);
	parse_file(data, av[1]);
	init_player(data);
	// convert_map_to_array(data);
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "cub3D");
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->img_addr = mlx_get_data_addr(data->img, &data->bpp, &data->size_line, &data->endien);
	mlx_hook(data->win, 17, 1L << 0, clean_exit, data);
	mlx_hook(data->win, 2, 1L << 0, on_keypress, data);
	mlx_hook(data->win, 3, 1L << 1, on_keyrelease, data);
	mlx_loop_hook(data->mlx, on_gameupdate, data);
	mlx_loop(data->mlx);
	return (0);
}
