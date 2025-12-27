/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 14:23:24 by yabarhda          #+#    #+#             */
/*   Updated: 2025/12/27 14:33:21 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/main.h"

static void	clear_window(t_data *data)
{
	int	i;

	i = 0;
	while (data->img_addr[i])
		data->img_addr[i++] = 0;
}

static void	draw_player(t_data *data)
{
	put_pixel(data, WIDTH / 2, HEIGHT / 2, 123445);
}

int	on_gameupdate(t_data *data)
{
	clear_window(data);
	move_player(data);
	draw_player(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	return (0);
}
