/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jait-chd <jait-chd@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 14:59:43 by yabarhda          #+#    #+#             */
/*   Updated: 2025/09/02 13:30:03 by jait-chd         ###   ########.fr       */
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
	len = ft_strlen(av[1]);
	if (ft_strcmp(".cub", &av[1][len - 4]))
		return (printf("Error\nInvalid file format\n"), close(fd), 0);
	return (1);
}

static void	init_data(t_data *data)
{
	data->mlx = NULL;
	data->map = NULL;
	data->img = NULL;
	data->size_line = 0;
	data->bpp = 0;
	data->endien = 0;
	data->width = 0;
	data->height = 0;
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
}

int	clean_exit(t_data *data)
{
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	ft_malloc(-42);
	exit(0);
}

void	draw_circle(t_data *data, int x, int y)
{
	float radius = 20;
	mlx_clear_window(data->mlx, data->win);
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			if ((i-50)*(i-50) + (j-50)*(j-50) <= radius*radius)
				mlx_pixel_put(data->mlx, data->win, x + j, y + i, 0x00FFFFFF);
		}
	}
}

int	key_hooks(int key, t_data *data)
{
	int x = data->player.x;
	int y = data->player.y;
	if (key == KEY_ESC)
		clean_exit(data);
	if (key == KEY_RIGHT)
		data->player.angle += 1.25;
	if (key == KEY_LEFT)
		data->player.angle -= 1.25;
	if (key == KEY_W)
	{
		data->player.y -= 3;
		y = data->player.y + 720 / 2;
		draw_circle(data, x, y);
	}
	if (key == KEY_A)
	{
		data->player.x -= 3;
		x = data->player.x + 1280 / 2;
		draw_circle(data, x, y);
	}
	if (key == KEY_S)
	{
		data->player.y += 3;
		y = data->player.y + 720 / 2;
		draw_circle(data, x, y);
	}
	if (key == KEY_D)
	{
		data->player.x += 3;
		x = data->player.x + 1280 / 2;
		draw_circle(data, x, y);
	}
	return (0);
}

void	init_player(t_data *data)
{
	if (data->player.direction == 'N')
		data->player.angle = 0;
	else if (data->player.direction == 'E')
		data->player.angle = 90;
	else if (data->player.direction == 'S')
		data->player.angle = 180;
	else if (data->player.direction == 'W')
		data->player.angle = 270;
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
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, 1280, 720, "cub3D");
	// mlx_get_data_addr(data->img , data->bpp , data->size_line , data->endien);
	draw_circle(data, data->player.x + 1280 / 2, data->player.y + 720 / 2);
	mlx_hook(data->win, 17, 1L << 0, clean_exit, data);
	mlx_hook(data->win, 2, 1L << 0, key_hooks, data);
	mlx_loop(data->mlx);
	return (0);
}
