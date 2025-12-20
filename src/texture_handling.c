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
        wall_tex.pxl_arr = mlx_get_data_addr(wall_tex.img, &wall_tex.bits_per_pixel,
            &wall_tex.line_len, &wall_tex.endian);
        wall_tex.bytes_per_pixel = wall_tex.bits_per_pixel / 8;
        cub->col->wall_tex[i] = wall_tex;
        i++;
    }
}

/*
t_tex select_texture(t_cub *cub, t_pos ray_dir, int is_horiz_hit)
{
    t_tex wall_tex;

    if (is_horiz_hit)
    {
        if (ray_dir.x > 0)
            wall_tex = cub->col->wall_tex[1];
        else
            wall_tex = cub->col->wall_tex[3];
    }
    else
    {
        if (ray_dir.y > 0)
            wall_tex = cub->col->wall_tex[2];
        else
            wall_tex = cub->col->wall_tex[0];
    }
    return (wall_tex);
}*/

t_tex select_texture(t_cub *cub, t_pos ray_dir, int is_horiz_hit)
{
    if (is_horiz_hit) // horizontal wall (north/south)
    {
        if (ray_dir.y > 0)
            return cub->col->wall_tex[2];
        else
            return cub->col->wall_tex[0];
    }
    else // vertical wall (east/west)
    {
        if (ray_dir.x > 0)
            return cub->col->wall_tex[1];
        else
            return cub->col->wall_tex[3];
    }
}


/* todo ?
void flip_texture(t_cub *cub, t_hit *hit, t_tex texture)
{
    if ((hit->is_horiz == 0 && hit->ray_dir.x < 0) ||
    (hit->is_horiz == 1 && hit->ray_dir.y > 0))
    {
        texture = texture.width - texture. - 1;
    }

}*/

//todo rm
//Texture X = hit->wall_x * texture.width
//Texture Y = (screen_y - wall_start) / wall_height * texture.height
int get_texture_px_color(t_hit *hit, float wall_height, int wall_start, int y, int clipped)
{
    char	*pixel;

    int rel_pos_y;

    rel_pos_y =  (int)(((float)(y + clipped - wall_start) / wall_height ) * (float)hit->tex.height);
    rel_pos_y = (int)fmin(fmax(rel_pos_y, 0), hit->tex.height-1);
    char *line_start = hit->tex.pxl_arr + rel_pos_y * hit->tex.line_len;
    pixel = line_start + (hit->rel_pos_x * (hit->tex.bytes_per_pixel));
    return *(int *)pixel;
}


