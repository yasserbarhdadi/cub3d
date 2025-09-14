/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 14:59:43 by yabarhda          #+#    #+#             */
/*   Updated: 2025/09/14 10:39:32 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/main.h"

static void	error_check(int ac, char **av)
{
	int	fd;
	int	len;

	if (ac != 2)
		(ft_perror("Not enough arguments"), exit(1));
	fd = open(av[1], O_RDONLY);
	if (fd == -1)
		(ft_perror("No such file or directory"), exit(1));
	len = ft_strlen(av[1]);
	if (ft_strcmp(".cub", &av[1][len - 4]))
		(ft_perror("Invalid file format"), exit(1));
}

static void	init_data(t_data *data)
{
	data->mlx = NULL;
	data->win = NULL;
	data->img = ft_malloc(sizeof(t_img));
	data->img->img = NULL;
	data->img->pixels = NULL;
	data->img->file = NULL;
	data->data = NULL;
	data->map = NULL;
	data->grid = NULL;
	data->texture = ft_malloc(sizeof(t_texture));
	data->texture->north = ft_malloc(sizeof(t_img));
	data->texture->north->file = NULL;
	data->texture->south = ft_malloc(sizeof(t_img));
	data->texture->south->file = NULL;
	data->texture->west = ft_malloc(sizeof(t_img));
	data->texture->west->file = NULL;
	data->texture->east = ft_malloc(sizeof(t_img));
	data->texture->east->file = NULL;
	data->texture->floor = NULL;
	data->texture->ceiling = NULL;
	data->player.x = 0;
	data->player.y = 0;
	data->player.angle = 0;
	data->player.direction = 0;
}

static void	init_player(t_data *data)
{
	if (data->player.direction == 'N')
		data->player.angle = PI / 2;
	else if (data->player.direction == 'E')
		data->player.angle = 0;
	else if (data->player.direction == 'S')
		data->player.angle = (3 * PI) / 2;
	else if (data->player.direction == 'W')
		data->player.angle = PI;
	data->player.key_w = false;
	data->player.key_a = false;
	data->player.key_s = false;
	data->player.key_d = false;
	data->player.key_right = false;
	data->player.key_left = false;
}

int	main(int ac, char **av)
{
	t_data	*data;

	error_check(ac, av);
	data = ft_malloc(sizeof(t_data));
	init_data(data);
	parse_file(data, av[1]);
	init_player(data);
	init_cub3d(data);
	return (0);
}
