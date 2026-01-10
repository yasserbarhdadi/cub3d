/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_element.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jait-chd <jait-chd@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:18:58 by jait-chd          #+#    #+#             */
/*   Updated: 2026/01/10 07:50:29 by jait-chd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/main.h"

static int	only_newline_or_empty(char *s)
{
	int	i;

	if (!s)
		return (1);
	i = 0;
	while (s[i])
	{
		if (s[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

static void	assign_texture(char **arr, char **texture, int fd)
{
	if (*texture)
	{
		ft_malloc(-42);
		ft_perror("Multiple definitions of textures");
		close(fd);
		exit(1);
	}
	if (!arr[1] || (arr[2] && !only_newline_or_empty(arr[2])))
		(ft_malloc(-42), ft_perror("Invalid map"), close(fd), exit(1));
	if (arr[1][ft_strlen(arr[1]) - 1] == '\n')
	{
		*texture = ft_malloc(sizeof(char) * ft_strlen(arr[1]));
		ft_strlcpy(*texture, arr[1], ft_strlen(arr[1]));
	}
	else
		*texture = arr[1];
}

static int	value_check(char *s, int fd)
{
	int	i;
	int	result;

	if (!s || s[0] == '\0' || s[0] == '\n')
		(ft_malloc(-42), ft_perror("Invalid map"), close(fd), exit(1));
	i = 0;
	result = 0;
	if (ft_strlen(s) > 4)
		(ft_malloc(-42), ft_perror("Invalid map"), close(fd), exit(1));
	while (s[i])
	{
		if (!ft_isdigit(s[i]) && s[i] != '\n')
			(ft_malloc(-42), ft_perror("Invalid map"), close(fd), exit(1));
		i++;
	}
	result = ft_atoi(s);
	if (result < 0 || result > 255)
		(ft_malloc(-42), ft_perror("Invalid map"), close(fd), exit(1));
	return (result);
}

static int	is_space_only(char *s)
{
	int	i;

	if (!s)
		return (1);
	i = 0;
	while (s[i])
	{
		if (!ft_isspace(s[i]))
			return (0);
		i++;
	}
	return (1);
}

static char	*trim_spaces(char *s)
{
	int	len;

	while (*s && ft_isspace(*s))
		s++;
	len = ft_strlen(s);
	while (len > 0 && ft_isspace(s[len - 1]))
	{
		s[len - 1] = '\0';
		len--;
	}
	return (s);
}

static char	*join_color_tokens(char **arr)
{
	char	*joined;
	char	*tmp;
	int		i;

	joined = ft_strdup("");
	i = 1;
	while (arr[i])
	{
		if (!is_space_only(arr[i]))
		{
			tmp = joined;
			joined = ft_strjoin(joined, arr[i]);
			(void)tmp;
		}
		i++;
	}
	return (joined);
}

static int	valid_color_format(char *s)
{
	int	commas;
	int	i;
	char	prev;

	if (!s)
		return (0);
	commas = 0;
	prev = 0;
	i = 0;
	while (s[i])
	{
		if (ft_isspace(s[i]))
		{
			i++;
			continue ;
		}
		if (s[i] == ',')
		{
			if (prev == ',' || prev == 0)
				return (0);
			commas++;
		}
		else if (!ft_isdigit(s[i]))
			return (0);
		prev = s[i];
		i++;
	}
	if (prev == ',')
		return (0);
	return (commas == 2);
}

static void	assign_colors(char **arr, unsigned char **texture, int fd)
{
	int		i;
	char	**tmp_arr;
	char	*colors;
	char	*tok;

	if (*texture)
	{
		ft_malloc(-42);
		ft_perror("Multiple definitions of floor colors");
		close(fd);
		exit(1);
	}
	if (!arr[1])
		(ft_malloc(-42), ft_perror("Invalid map"), close(fd), exit(1));
	colors = join_color_tokens(arr);
	colors = trim_spaces(colors);
	if (!valid_color_format(colors))
		(ft_malloc(-42), ft_perror("Invalid map"), close(fd), exit(1));
	tmp_arr = ft_split(colors, ',');
	i = 0;
	*texture = ft_malloc(sizeof(unsigned char) * 3);
	while (tmp_arr[i])
	{
		tok = trim_spaces(tmp_arr[i]);
		if (tok[0] == '\0' || tok[0] == '\n' || i == 3)
			(ft_malloc(-42), ft_perror("Invalid map"), close(fd), exit(1));
		(*texture)[i] = value_check(tok, fd);
		i++;
	}
	if (i != 3 || tmp_arr[i] != NULL)
		(ft_malloc(-42), ft_perror("Invalid map"), close(fd), exit(1));
}

void	check_element(t_data *data, char **arr, int fd)
{
	if (!ft_strcmp(arr[0], "NO"))
		assign_texture(arr, &data->texture->north, fd);
	else if (!ft_strcmp(arr[0], "SO"))
		assign_texture(arr, &data->texture->south, fd);
	else if (!ft_strcmp(arr[0], "WE"))
		assign_texture(arr, &data->texture->west, fd);
	else if (!ft_strcmp(arr[0], "EA"))
		assign_texture(arr, &data->texture->east, fd);
	else if (!ft_strcmp(arr[0], "F"))
		assign_colors(arr, &data->texture->floor, fd);
	else if (!ft_strcmp(arr[0], "C"))
		assign_colors(arr, &data->texture->ceiling, fd);
	else
		(ft_malloc(-42), ft_perror("Invalid map"), close(fd), exit(1));
}

void	convert_map_to_array(t_data *data)
{
	t_map	*current;
	int		i;

	current = data->map;
	while (current)
	{
		data->map_height++;
		if (ft_strlen(current->row) > (size_t)data->map_width)
			data->map_width = ft_strlen(current->row);
		current = current->next;
	}
	data->map_array = ft_malloc(sizeof(char *) * (data->map_height + 1));
	current = data->map;
	i = 0;
	while (current)
	{
		data->map_array[i++] = current->row;
		current = current->next;
	}
	data->map_array[i] = NULL;
}
