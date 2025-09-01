/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jait-chd <jait-chd@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 15:21:01 by jait-chd          #+#    #+#             */
/*   Updated: 2025/09/01 15:41:56 by jait-chd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/main.h"

void    load_single_texture(t_data *data , char *path)
{

    data->img = mlx_xpm_file_to_image(data->mlx, path, data->width, data->width);
    mlx_get_data_addr(data->img, data->bpp, data->size_line, data->endien);
}

void load_textures(t_data *data , char *path) {
    
}