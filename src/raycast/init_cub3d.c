/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cub3d.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:54:52 by yabarhda          #+#    #+#             */
/*   Updated: 2025/09/06 14:39:45 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/main.h"

int player_color;
int map_color;


static int	clean_exit(t_data *data)
{
	mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	ft_malloc(-42);
	exit(0);
}

static int	key_press(int key, t_data *data)
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

static int	key_release(int key, t_data *data)
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

static void put_pixel(t_data *data, int x, int y, int color)
{
	if (x >= WIDTH || x < 0 || y > HEIGHT || y < 0)
		return ;
	int offset = (y * data->size_line) + (x * 4);
	data->data[offset] = (color >> 24) & 0xFF; // blue
	data->data[offset + 1] = (color >> 16) & 0xFF; // green
	data->data[offset + 2] = (color >> 8) & 0xFF; // red
	data->data[offset + 3] = (color) & 0xFF;
}

static void draw_box(t_data *data, int x, int y, int color, int size)
{
	for (int i = 0; i < size; i++)
		put_pixel(data, x + i, y, color);
	for (int i = 0; i < size; i++)
		put_pixel(data, x, y + i, color);
	for (int i = 0; i <= size; i++)
		put_pixel(data, x + i, y + size, color);
	for (int i = 0; i < size; i++)
		put_pixel(data, x + size, y + i, color);
}

static void move_player(t_data *data)
{
	float speed = 1;
	float rot_speed = 0.03;

	if (data->player.angle > PI * 2)
		data->player.angle = 0;
	if (data->player.angle < 0)
		data->player.angle = PI * 2;
	
	float cos_angle = cos(data->player.angle);
	float sin_angle = sin(data->player.angle);
	
	if (data->player.key_w)
	{
		data->player.x += cos_angle;
		data->player.y += sin_angle;
	}
	if (data->player.key_a)
	{
		data->player.x += cos_angle + (PI / 2);
		data->player.y += sin_angle + (PI / 2);		
	}
	if (data->player.key_s)
	{
		data->player.x -= cos_angle;
		data->player.y -= sin_angle;
	}
	if (data->player.key_d) data->player.x += speed;
	if (data->player.key_right) data->player.angle += rot_speed;
	if (data->player.key_left) data->player.angle -= rot_speed;
}

static void clear_window(t_data *data)
{
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
			put_pixel(data, x, y, 0);
}

static void	draw_map(t_data *data, int color)
{
	int x_axis = 0;
	int y_axis = 0;
	for (int y = 0; data->grid[y]; y++)
	{
		x_axis = 0;
		for (int x = 0; data->grid[y][x]; x++)
		{
			if (data->grid[y][x] == '1')
			{
				draw_box(data, x_axis, y_axis, color, 32);
			}
			x_axis+=32;
		}
		y_axis+=32;
	}
}

static void	draw_line(t_data *data)
{
	int line_len = 30;
	float cos_angle = cos(data->player.angle);
	float sin_angle = sin(data->player.angle);
	for (int i = 0; i < line_len; i++)
	{
		put_pixel(data, data->player.x + 8 + (cos_angle * i), data->player.y + 8 + (sin_angle * i), decode_rgb(255, 0, 0));
	}
}

static void draw_player(t_data *data)
{
	draw_box(data, data->player.x, data->player.y, player_color, 16);
	draw_line(data);
}

static int	on_game_update(t_data *data)
{
	clear_window(data);
	move_player(data);
	draw_map(data, map_color);
	draw_player(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	return (0);
}

void	init_cub3d(t_data *data)
{
	player_color = decode_rgb(0, 255, 30);
	map_color = decode_rgb(128, 128, 128);
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "cub3D");
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->grid = ft_lsttoarr(data->map);
	data->data = \
	mlx_get_data_addr(data->img, &data->bpp, &data->size_line, &data->endian);
	put_pixel(data, WIDTH / 2, HEIGHT / 2, player_color);
	mlx_hook(data->win, 17, 1L << 0, clean_exit, data);
	mlx_hook(data->win, 2, 1L << 0, key_press, data);
	mlx_hook(data->win, 3, 1L << 1, key_release, data);
	mlx_loop_hook(data->mlx, on_game_update, data);
	mlx_loop(data->mlx);
}
