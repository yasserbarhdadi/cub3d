/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_drawing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jait-chd <jait-chd@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:25:59 by jait-chd          #+#    #+#             */
/*   Updated: 2025/09/08 19:58:56 by jait-chd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../inc/main.h"

void check_x_y(int x, int y) {
	if(x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return;
}

void	put_pixel(t_data *data, int x, int y, int color)
{
	char	*dst;

	check_x_y(x , y);
	dst = data->img_addr + (y * data->size_line + x * (data->bpp / 8));
	*(unsigned int*)dst = color;
}

void	draw_vertical_line(t_data *data, int x, int start, int end, int color)
{
	int	y;

	y = start;
	while (y < end)
	{
		put_pixel(data, x, y, color);
		y++;
	}
}

static void	draw_wall_slice(t_data *data, t_ray *ray, int x, int line_height)
{
	int	draw_start;
	int	draw_end;
	int	color;

	draw_start = -line_height / 2 + HEIGHT / 2;
	if (draw_start < 0)
		draw_start = 0;
	draw_end = line_height / 2 + HEIGHT / 2;
	if (draw_end >= HEIGHT)
		draw_end = HEIGHT - 1;
	if (ray->side == 0)
		color = 0xFF0000;
	else
		color = 0x00FF00;
	draw_vertical_line(data, x, draw_start, draw_end, 0x0FFFFF);
	draw_vertical_line(data, x, 0, draw_start, 0x0263238);
	draw_vertical_line(data, x, draw_end, HEIGHT, 0x0263238);
}

void	raycast(t_data *data)
{
	t_ray	ray;
	int		x;
	int		line_height;

	x = 0;
	while (x < WIDTH)
	{
		cast_ray(data, &ray, x);
		line_height = (int)(HEIGHT / ray.perp_wall_dist);
		draw_wall_slice(data, &ray, x, line_height);
		x++;
	}
}
