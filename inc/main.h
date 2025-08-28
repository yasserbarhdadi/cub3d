/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabarhda <yabarhda@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 15:00:04 by yabarhda          #+#    #+#             */
/*   Updated: 2025/08/28 18:44:08 by yabarhda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "/home/yabarhda/Documents/mlx/mlx.h"

# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100

typedef struct s_mem
{
	void			*ptr;
	struct s_mem	*next;
}					t_mem;

typedef struct s_texture
{
	void			*north;
	void			*south;
	void			*west;
	void			*east;
	unsigned char	floor[3];
	unsigned char	ceiling[3];
}					t_texture;

typedef struct s_map
{
	// something
}					t_map;

typedef struct s_data
{
	void			*mlx;
	void			*win;
	t_map			*map;
	t_texture		*texture;
}					t_data;

int		ft_strcmp(const char *s1, const char *s2);

char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strdup(const char *s);
char	*get_next_line(int fd);
char	**ft_split(char const *s, char c);

void	*ft_malloc(long size);

size_t	ft_strlen(const char *s);
size_t	ft_strlcat(char *dest, const char *src, size_t size);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);

#endif