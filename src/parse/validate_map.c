/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 16:50:38 by yabarhda          #+#    #+#             */
/*   Updated: 2025/12/26 05:48:17 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/main.h"

static void	standard_check(t_data *data)
{
	int (x), y;
	t_map *(tmp);
	y = 0;
	tmp = data->map;
	while (tmp)
	{
		x = 0;
		while (tmp->row[x])
		{
			if (ft_isplayer(tmp->row[x]))
			{
				if (data->player.x)
					(ft_malloc(-42), ft_perror("Invalid map"), exit(1));
				data->player.x = x + 0.5;
				data->player.y = y + 0.5;
				data->player.direction = tmp->row[x];
				tmp->row[x] = '0';
			}
			x++;
		}
		y++;
		tmp = tmp->next;
	}
	if (!data->player.x)
		(ft_malloc(-42), ft_perror("Player not found"), exit(1));
}

static void	check_borders(char **m)
{
	int (j), i = 0;
	while (m[i])
	{
		j = 0;
		while (m[i][j])
		{
			if (m[i][j] == '0')
			{
				if (!m[i][j + 1] || (m[i][j + 1] != '0'
					&& m[i][j + 1] != '1'))
					early_exit("Invalid map");
				if (j == 0 || !m[i][j - 1]
					|| (m[i][j - 1] != '0' && m[i][j - 1] != '1'))
					early_exit("Invalid map");
				if (!m[i + 1] || j >= (int)ft_strlen(m[i + 1]) || !m[i + 1][j]
				|| (m[i + 1][j] != '0' && m[i + 1][j] != '1'))
					early_exit("Invalid map");
				if (i == 0 || j >= (int)ft_strlen(m[i - 1]) || !m[i - 1][j]
				|| (m[i - 1][j] != '0' && m[i - 1][j] != '1'))
					early_exit("Invalid map");
			}
			j++;
		}
		i++;
	}
}

static void	deep_check(t_data *data)
{
	int		i;
	char	**map;
	t_map	*tmp;

	map = ft_malloc(sizeof(char *) * (data->map->size + 1));
	map[data->map->size] = NULL;
	tmp = data->map;
	i = 0;
	while (tmp)
	{
		map[i] = ft_strdup(tmp->row);
		if (!map[i][0])
			(ft_malloc(-42), ft_perror("Invalid map"), exit(1));
		tmp = tmp->next;
		i++;
	}
	check_borders(map);
}

static void	get_map_size(t_data *data)
{
	t_map	*tmp;
	int		size;

	tmp = data->map;
	size = 0;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	tmp = data->map;
	while (tmp)
	{
		tmp->size = size;
		tmp = tmp->next;
	}
}

void	validate_map(t_data *data)
{
	get_map_size(data);
	standard_check(data);
	deep_check(data);
}
