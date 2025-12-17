/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:17:12 by mdiederi          #+#    #+#             */
/*   Updated: 2025/12/16 10:17:15 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void create_texture_imgs(t_cub *cub)
{
    int i;
    char *file_paths[4];
    t_tex wall_tex;

    file_paths[0] = cub->col->no_tex_p;
    file_paths[1] = cub->col->ea_tex_p;
    file_paths[2] = cub->col->so_tex_p;
    file_paths[3] = cub->col->we_tex_p;
    i = 0;
    while (i < 4)
    {
        wall_tex.img = mlx_xpm_file_to_image(cub->mlx, file_paths[i],
                            &wall_tex.width, &wall_tex.height);
        if (!wall_tex.img)
            error_exit(cub, "Error\nFailed to load texture", NULL);
        wall_tex.pxl_arr = mlx_get_data_addr(wall_tex.img, &wall_tex.bpp,
            &wall_tex.line_len, &wall_tex.endian);
        cub->col->wall_tex[i] = wall_tex;
        i++;
    }
}

//todo how to get right tile ?

int get_texture_px_color(t_cub *cub, float wall_height, t_pos pos)
{
    t_tex sample_tex;
    int color;
    int rel_pos_x;
    int rel_pos_y;

    color = 0;
    rel_pos_x = pos.x/sample_tex.width;
    rel_pos_y = pos.y/sample_tex.height;
    sample_tex = cub->col->wall_tex[0];
    if (cub->tile_size == sample_tex.height)
    {
        color = sample_tex.pxl_arr[rel_pos_y][rel_pos_x];
        color = cub->pxl_arr + (y * cub->mlx_data.line_length) + (x
                * (cub->mlx_data.bits_per_pixel / 8));
    }
    else
    {

    }
    return color;
}