/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jait-chd <jait-chd@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:21:32 by jait-chd          #+#    #+#             */
/*   Updated: 2026/01/10 07:45:13 by jait-chd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/main.h"

void	add_node(t_map **map, t_map *new)
{
	t_map		*temp;

	if (!*map)
	{
		*map = new;
		return ;
	}
	temp = *map;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

t_map	*new_node(char *row)
{
	t_map		*new;

	new = ft_malloc(sizeof(t_map));
	if (row[ft_strlen(row) - 1] == '\n')
	{
		new->row = ft_malloc(sizeof(char) * ft_strlen(row));
		ft_strlcpy(new->row, row, ft_strlen(row));
	}
	else
		new->row = row;
	new->next = NULL;
	return (new);
}

int	valid_map_char(char c)
{
	if (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'W' || c == 'E')
		return (1);
	return (0);
}

static int	only_spaces(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != '\n' && !ft_isspace(line[i]))
			return (0);
		i++;
	}
	return (1);
}

void	map_check(t_data *data, int fd)
{
	int		i;
	char	*line;

	line = get_next_line(fd);
	while (line && only_spaces(line))
		line = get_next_line(fd);
	while (line)
	{
		if (only_spaces(line))
		{
			line = get_next_line(fd);
			while (line)
			{
				if (!only_spaces(line))
					(ft_malloc(-42), close(fd), ft_perror("Invalid map"), exit(1));
				line = get_next_line(fd);
			}
			break ;
		}
		i = 0;
		while (line[i])
		{
			if (!ft_isspace(line[i]) && !valid_map_char(line[i]))
				(ft_malloc(-42), close(fd), ft_perror("Invalid map"), \
				exit(1));
			i++;
		}
		add_node(&data->map, new_node(line));
		line = get_next_line(fd);
	}
	close(fd);
	validate_map(data);
}

void	early_exit(char *error)
{
	ft_malloc(-42);
	ft_perror(error);
	exit(1);
}
