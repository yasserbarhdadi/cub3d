/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 14:59:43 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/28 18:55:28 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/main.h"

int	error_check(int ac, char **av)
{
	int		fd;
	int		len;

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
	data->texture = NULL;
}

static int	parse_file(t_data *data, char *file)
{
	int		fd;
	char	*line;
	(void)data;

	fd = open(file, O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		char **arr_line = ft_split(line, ' ');
		if (arr_line[0][0] == '\n')
		{
			line = get_next_line(fd);
			continue ;
		}
		// check_element(data, arr_line);
		line = get_next_line(fd);
	}
	return (1);
}

int	clean_exit(t_data *data)
{
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	ft_malloc(-1337);
	exit(0);
}

int	main(int ac, char **av)
{
	t_data	*data;

	if (!error_check(ac, av))
		return (1);
	data = ft_malloc(sizeof(t_data));
	init_data(data);
	if (!parse_file(data, av[1]))
		return (ft_malloc(-1337), 1);
	data->mlx = mlx_init();
	data->win = mlx_new_window(data->mlx, 1280, 720, "cub3D");
	mlx_hook(data->win, 17, 1L<<0, clean_exit, data);
	mlx_loop(data->mlx);
	return (0);	
}
