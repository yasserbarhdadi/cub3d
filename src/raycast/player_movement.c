/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 14:22:36 by yabarhda          #+#    #+#             */
/*   Updated: 2025/12/27 14:32:35 by yabarhda         ###   ########.fr       */
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
