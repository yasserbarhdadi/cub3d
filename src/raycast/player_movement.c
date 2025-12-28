/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 14:22:36 by yabarhda          #+#    #+#             */
/*   Updated: 2025/12/29 00:02:16 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/main.h"

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
	float movespeed = 0.5;
	float rotation_speed = 0.025;
	if (data->player.key_w && (data->player.y - movespeed) > 0)
	{
		data->player.x = data->player.x + (movespeed * cos(data->player.angle));
		data->player.y = data->player.y + (movespeed * sin(data->player.angle));
	}
	if (data->player.key_s && (data->player.y + movespeed + 7) < HEIGHT)
	{
		data->player.x = data->player.x - (movespeed * cos(data->player.angle));
		data->player.y = data->player.y - (movespeed * sin(data->player.angle));
	}
	if (data->player.key_a && (data->player.x - movespeed) > 0)
	{
		data->player.x = data->player.x - (movespeed * cos(data->player.angle + (PI / 2)));
		data->player.y = data->player.y - (movespeed * sin(data->player.angle + (PI / 2)));
	}
	if (data->player.key_d && (data->player.x + movespeed + 7) < WIDTH)
	{
		data->player.x = data->player.x + (movespeed * cos(data->player.angle + (PI / 2)));
		data->player.y = data->player.y + (movespeed * sin(data->player.angle + (PI / 2)));
	}
	if (data->player.key_left)
	{
		data->player.angle -= rotation_speed;
		if (data->player.angle <= 0)
			data->player.angle += 2 * PI;
	}
	if (data->player.key_right)
	{
		data->player.angle += rotation_speed;
		if (data->player.angle >= 2 * PI)
			data->player.angle -= 2 * PI;
	}
}
