/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_texture_handling.c                            :+:      :+:    :+:   */
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
            error_exit(cub, "Failed to load texture", NULL);
        wall_tex.pxl_arr = mlx_get_data_addr(wall_tex.img, &wall_tex.bits_per_pixel,
            &wall_tex.line_len, &wall_tex.endian);
        wall_tex.bytes_per_pixel = wall_tex.bits_per_pixel / 8;
        cub->col->wall_tex[i] = wall_tex;
        i++;
    }
}

// horziontal walls = north & south
// vertical walls = east & west
t_tex select_texture(t_cub *cub, t_pos ray_dir, int is_horiz_hit)
{
    if (is_horiz_hit)
    {
        if (ray_dir.y > 0)
            return cub->col->wall_tex[0];
        return cub->col->wall_tex[2];
    }
    if (ray_dir.x > 0)
        return cub->col->wall_tex[3];
    return cub->col->wall_tex[1];
}
