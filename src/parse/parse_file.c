/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:17:00 by yabarhda          #+#    #+#             */
/*   Updated: 2026/01/10 02:01:06 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/main.h"

static void	final_check(t_data *data, int fd)
{
	if (!data->texture->north || !data->texture->west || !data->texture->east \
		|| !data->texture->south || !data->texture->floor \
		|| !data->texture->ceiling)
	{
		ft_malloc(-42);
		ft_perror("Invalid map");
		close(fd);
		exit(1);
	}
}

static void	check_one_texture(char *path, int f)
{
	char	buf[1];
	int		len;
	int		fd;

	len = ft_strlen(path);
	if (len < 5 || ft_strncmp(path + len - 4, ".xpm", 4))
		(ft_malloc(-42), ft_perror("Invalid texture file"), close(f), exit(1));
	fd = open(path, O_RDONLY);
	if (fd < 0 || read(fd, buf, 1) <= 0)
		(ft_malloc(-42), ft_perror("Invalid texture file"), close(f), exit(1));
	close(fd);
}

static void	check_texture_file(t_data *data, int f)
{
	check_one_texture(data->texture->north, f);
	check_one_texture(data->texture->west, f);
	check_one_texture(data->texture->south, f);
	check_one_texture(data->texture->east, f);
}

static int	validate_elements(t_data *data)
{
	if (!data->texture->north || !data->texture->west || !data->texture->east \
		|| !data->texture->south || !data->texture->floor \
		|| !data->texture->ceiling)
	{
		return (0);
	}
	return (1);
}

void	parse_file(t_data *data, char *file)
{
	int		fd;
	char	*line;
	char	**arr_line;

	fd = open(file, O_RDONLY);
	line = get_next_line(fd);
	if (!line)
		(ft_malloc(-42), close(fd), ft_perror("Empty file"), exit(1));
	while (line)
	{
		arr_line = ft_split(line, ' ');
		if (arr_line[0][0] == '\n')
		{
			line = get_next_line(fd);
			continue ;
		}
		check_element(data, arr_line, fd);
		if (validate_elements(data))
			break ;
		line = get_next_line(fd);
	}
	final_check(data, fd);
	check_texture_file(data, fd);
	map_check(data, fd);
}
