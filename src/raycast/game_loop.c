/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 14:23:24 by yabarhda          #+#    #+#             */
/*   Updated: 2025/12/28 23:54:02 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/main.h"

static void	clear_window(t_data *data)
{
	int	i;

	i = 0;
	while (i < (HEIGHT * data->size_line))
		data->img_addr[i++] = 0;
}

void draw_antena(t_data *data)
{
	for (int i = 0; i < 20; i++)
	{
		put_pixel(data, (int)(data->player.x + (i * cos(data->player.angle))), (int)(data->player.y + (i * sin(data->player.angle))), 0xFFFF0000);
	}
}

static void	draw_player(t_data *data)
{
	int playersize = 7;
	for (int i = 0; i < playersize; i++)
	{
		for (int j = 0; j < playersize; j++)
			put_pixel(data, data->player.x + i, data->player.y + j, 123445);
	}
	draw_antena(data);
}

void draw_box(t_data *data, int x, int y)
{
	for (int i = 0; i < BLOCK; i++)
	{
		for (int j = 0; j < BLOCK; j++)
		{
			if (j == 0 || i == 0 || j == BLOCK - 1 || i == BLOCK - 1)
				put_pixel(data, x + i, y + j, 123445);
		}
	}
}

void draw_background_block(t_data *data, int x, int y)
{
	for (int i = 0; i < BLOCK; i++)
	{
		for (int j = 0; j < BLOCK; j++)
		{
			put_pixel(data, x + i, y + j, 0xFFFFFFFF);
		}
	}	
}

void draw_map(t_data *data)
{
	for (int i = 0; data->map_array[i]; i++)
	{
		for (int j = 0; data->map_array[i][j]; j++)
		{
			if (data->map_array[i][j] == '1')
				draw_box(data, j * BLOCK, i * BLOCK);
			else if (data->map_array[i][j] == '0')
				draw_background_block(data, j * BLOCK, i * BLOCK);
		}
	}
}

int	on_gameupdate(t_data *data)
{
	clear_window(data);
	move_player(data);
	draw_map(data);
	draw_player(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	return (0);
}
