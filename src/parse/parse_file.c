/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:17:00 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/30 15:23:14 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/main.h"

static void	final_check(t_data *data)
{
	if (!data->texture->north || !data->texture->west || !data->texture->east \
		|| !data->texture->south || !data->texture->floor \
		|| !data->texture->ceiling)
	{
		ft_malloc(-1337);
		ft_perror("Invalid texture file");
		exit(1);
	}
}

static void	check_texture_file(t_data *data)
{
	int	fd;

	fd = open(data->texture->north, O_RDONLY);
	if (fd == -1)
		(ft_malloc(-1337), ft_perror("Invalid texture file"), exit(1));
	close(fd);
	fd = open(data->texture->west, O_RDONLY);
	if (fd == -1)
		(ft_malloc(-1337), ft_perror("Invalid texture file"), exit(1));
	close(fd);
	fd = open(data->texture->south, O_RDONLY);
	if (fd == -1)
		(ft_malloc(-1337), ft_perror("Invalid texture file"), exit(1));
	close(fd);
	fd = open(data->texture->east, O_RDONLY);
	if (fd == -1)
		(ft_malloc(-1337), ft_perror("Invalid texture file"), exit(1));
	close(fd);
}

int	parse_file(t_data *data, char *file)
{
	int		fd;
	char	*line;
	char	**arr_line;

	fd = open(file, O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		arr_line = ft_split(line, ' ');
		if (arr_line[0][0] == '\n')
		{
			line = get_next_line(fd);
			continue ;
		}
		check_element(data, arr_line);
		line = get_next_line(fd);
	}
	close(fd);
	final_check(data);
	check_texture_file(data);
	map_check(data);
	return (1);
}
