/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lsttoarr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 21:09:36 by yabarhda          #+#    #+#             */
/*   Updated: 2025/09/05 21:14:50 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/main.h"

char	**ft_lsttoarr(void *lst)
{
	int		i;
	char	**arr;
	t_map	*tmp;

	i = 0;
	tmp = (t_map *)lst;
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}
	arr = ft_malloc((i + 1) * sizeof(char *));
	arr[i] = NULL;
	tmp = (t_map *)lst;
	while (tmp)
	{
		arr[i++] = tmp->row;
		tmp = tmp->next;
	}
	return (arr);
}
