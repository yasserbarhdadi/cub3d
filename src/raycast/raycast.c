/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 16:09:24 by yabarhda          #+#    #+#             */
/*   Updated: 2025/12/30 16:25:49 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/main.h"

static void	init_ray_coords(t_data *data)
{
	data->ray.map_x = data->player.x / BLOCK;
	data->ray.map_y = data->player.y / BLOCK;
	data->ray.pos_x = data->player.x / BLOCK;
	data->ray.pos_y = data->player.y / BLOCK;
	data->ray.delta_x = fabs(1 / cos(data->ray.angle));
	data->ray.delta_y = fabs(1 / sin(data->ray.angle));
}

static void	init_ray_step(t_data *data)
{
	init_ray_coords(data);
	if (cos(data->ray.angle) < 0)
	{
		data->ray.step_x = -1;
		data->ray.side_x = (data->ray.pos_x - data->ray.map_x) * \
		data->ray.delta_x;
	}
	else
	{
		data->ray.step_x = 1;
		data->ray.side_x = (data->ray.map_x + 1 - data->ray.pos_x) * \
		data->ray.delta_x;
	}
	if (sin(data->ray.angle) < 0)
	{
		data->ray.step_y = -1;
		data->ray.side_y = (data->ray.pos_y - data->ray.map_y) * \
		data->ray.delta_y;
	}
	else
	{
		data->ray.step_y = 1;
		data->ray.side_y = (data->ray.map_y + 1 - data->ray.pos_y) * \
		data->ray.delta_y;
	}
}

static void	dda(t_data *data)
{
	while (data->map_array[data->ray.map_y][data->ray.map_x] != '1')
	{
		if (data->ray.side_x < data->ray.side_y)
		{
			data->ray.side_x += data->ray.delta_x;
			data->ray.map_x += data->ray.step_x;
			data->ray.side = 0;
		}
		else
		{
			data->ray.side_y += data->ray.delta_y;
			data->ray.map_y += data->ray.step_y;
			data->ray.side = 1;
		}
	}
}

static void draw_wall(t_data *data, int x)
{
	float	distance;
	float	perp_dis;
	float	wall_height;
	float	start;
	float	end;
	int		y;

	if (data->ray.side == 0)
		distance = data->ray.side_x - data->ray.delta_x;
	else
		distance = data->ray.side_y - data->ray.delta_y;
	perp_dis = distance * cos(data->ray.angle - data->player.angle);
	wall_height = HEIGHT / perp_dis; //(BLOCK / perp_dis) * (WIDTH / 2) / tan(FOV / 2); // HEIGHT / perp_dis

	start = -wall_height / 2 + HEIGHT / 2;
	if (start < 0)
		start = 0;
	end = wall_height / 2 + HEIGHT / 2;
	if (end >= HEIGHT)
		end = HEIGHT - 1;
	y = start;
	while (y < end)
	{
		put_pixel(data, x, y, 0xFFFF0000);
		y++;
	}	
}

static void	draw_ray_line(t_data *data)
{
	float	distance;
	int		i;
	int		px;
	int		py;

	if (data->ray.side == 0)
		distance = data->ray.side_x - data->ray.delta_x;
	else
		distance = data->ray.side_y - data->ray.delta_y;
	i = 0;
	while (i < distance * BLOCK)
	{
		px = (int)(data->player.x + (i * cos(data->ray.angle)));
		py = (int)(data->player.y + (i * sin(data->ray.angle)));
		put_pixel(data, px, py, 0xFFFF0000);
		i++;
	}
}

void	draw_rays(t_data *data)
{
	float	step;
	float	offset;
	int		j;

	offset = (FOV / 2) * (PI / 180);
	data->ray.angle = data->player.angle - offset;
	step = (FOV * (PI / 180)) / WIDTH;
	j = 0;
	while (j < WIDTH)
	{
		init_ray_step(data);
		dda(data);
		// draw_ray_line(data);
		draw_wall(data, j);
		data->ray.angle += step;
		j++;
	}
}
