/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_element.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:18:58 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/30 15:19:28 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/main.h"

static void	assign_texture(char **arr, char **texture)
{
	if (*texture)
	{
		ft_malloc(-1337);
		ft_perror("Multiple definitions of textures");
		exit(1);
	}
	if (!arr[1])
		(ft_malloc(-1337), ft_perror("Invalid map"), exit(1));
	if (arr[1][ft_strlen(arr[1]) - 1] == '\n')
	{
		*texture = ft_malloc(sizeof(char) * ft_strlen(arr[1]));
		ft_strlcpy(*texture, arr[1], ft_strlen(arr[1]));
	}
	else
		*texture = arr[1];
}

static void	assign_colors(char **arr, unsigned char **texture)
{
	int		i;
	char	**tmp_arr;

	if (*texture)
	{
		ft_malloc(-1337);
		ft_perror("Multiple definitions of floor colors");
		exit(1);
	}
	if (!arr[1])
		(ft_malloc(-1337), ft_perror("Invalid map"), exit(1));
	tmp_arr = ft_split(arr[1], ',');
	i = 0;
	*texture = ft_malloc(sizeof(unsigned char) * 3);
	while (tmp_arr[i])
	{
		if (i == 3)
			(ft_malloc(-1337), ft_perror("Invalid map"), exit(1));
		(*texture)[i] = ft_atoi(tmp_arr[i]);
		i++;
	}	
}

void	check_element(t_data *data, char **arr)
{
	if (!ft_strcmp(arr[0], "NO"))
		assign_texture(arr, &data->texture->north);
	else if (!ft_strcmp(arr[0], "SO"))
		assign_texture(arr, &data->texture->south);
	else if (!ft_strcmp(arr[0], "WE"))
		assign_texture(arr, &data->texture->west);
	else if (!ft_strcmp(arr[0], "EA"))
		assign_texture(arr, &data->texture->east);
	else if (!ft_strcmp(arr[0], "F"))
		assign_colors(arr, &data->texture->floor);
	else if (!ft_strcmp(arr[0], "C"))
		assign_colors(arr, &data->texture->ceiling);
}
