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


//Texture X = hit->wall_x * texture.width
//Texture Y = (screen_y - wall_start) / wall_height * texture.height
int get_texture_px_color(t_cub *cub, t_hit *hit, float wall_height, int wall_start, int y, int clipped)
{
    t_tex texture;
    char	*pixel;
    int rel_pos_x;
    int rel_pos_y;
    double wall_x;

    texture = select_texture(cub, hit->ray_dir, hit->is_horiz);
    if (hit->is_horiz == 0) // vertical wall
        wall_x = hit->hit_point.y - floorf(hit->hit_point.y);
    else                  // horizontal wall
        wall_x = hit->hit_point.x - floorf(hit->hit_point.x);
    rel_pos_x = wall_x * texture.width;
    if (!hit->is_horiz && hit->ray_dir.x < 0)
        rel_pos_x = texture.width - rel_pos_x - 1;
    if (hit->is_horiz && hit->ray_dir.y > 0)
        rel_pos_x = texture.width - rel_pos_x - 1;
    rel_pos_x = fmin(fmax(rel_pos_x, 0), texture.width-1);
    rel_pos_y =  (int)(((float)(y + clipped - wall_start) / wall_height ) * texture.height);
    rel_pos_y = fmin(fmax(rel_pos_y, 0), texture.height-1);
    //printf("wall_x=%f tex_x=%d tex_y=%d\n", wall_x, rel_pos_x, rel_pos_y);

    //printf("rel_pos_x: %d\nrel_pos y : %d\ntile width: %d\ntile height: %d\n", rel_pos_x, rel_pos_y, texture.width, texture.height);
    //if (rel_pos_y < texture.height && rel_pos_x < texture.width)
    pixel = texture.pxl_arr + (rel_pos_y * texture.line_len) + (rel_pos_x
    * (texture.bpp / 8));
    //printf("texture color found: %d\n", *(int *)pixel);
    return *(int *)pixel;
}


/*
int get_texture_px_color(t_cub *cub, float wall_height, t_pos pos)
{
    t_tex texture;
    char	*pixel;
    int rel_pos_x;
    int rel_pos_y;
    float height_ratio;

    texture = select_texture(cub, pos, 0);
    height_ratio = wall_height/texture.height;
    rel_pos_x = 0;
    rel_pos_y = 0;
    if (pos.x > 0)
        rel_pos_x = (pos.x/cub->tile_size) * texture.width;
    if (pos.y > 0)
        rel_pos_y = (int) (pos.y*height_ratio);
    printf("rel_pos_x: %d\nrel_pos y : %d\ntile width: %d\ntile height: %d\n", rel_pos_x, rel_pos_y, texture.width, texture.height);
    if (rel_pos_y < texture.height && rel_pos_x < texture.width)
    {
        pixel = texture.pxl_arr + (rel_pos_y * texture.line_len) + (rel_pos_x
    * (texture.bpp / 8));
    }


    if (cub->tile_size == sample_tex.height)
    {

    }
    else
    {

    }
    return *(int *)pixel;
}*/