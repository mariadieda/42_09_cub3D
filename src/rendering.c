/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 16:08:57 by mdiederi          #+#    #+#             */
/*   Updated: 2025/10/16 16:09:01 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../inc/cub3d.h"

void	clean_img(t_cub *cub, int color)
//todo replace one color with true map pixel colors
{
	char	*pixel;
	int		i;
	int		j;

	i = 0;
	while (i < cub->mlx_data.win_height)
	{
		j = 0;
		while (j < cub->mlx_data.win_width)
		{
			{
				pixel = cub->pxl_arr + (i * cub->mlx_data.line_length) + (j
						* (cub->mlx_data.bits_per_pixel / 8));
				*(unsigned int *)pixel = color;
			}
			j++;
		}
		i++;
	}
}

typedef struct s_dda
{
	int map_x;       // tile X
	int map_y;       // tile Y
	int side;        // 0 = vertical, 1 = horizontal
	float perp_dist; // distance from player to wall
	float hit_x;     // exact hit position in tile (for texture)
	float hit_y;
} t_dda;


/*
 *
* ray_dir	step
ray_dir.x < 0	go left
ray_dir.x > 0	go right
ray_dir.y < 0	go up
ray_dir.y > 0	go down
 */
t_dda dda_ray(t_cub *cub, float ray_angle)
{
    t_dda hit;
	t_int_pos map_pos;
	t_int_pos step;
	t_pos player_float_map_pos;
	t_pos ray_dir;
	t_pos delta_dist;
	t_pos side_dist;

	map_pos = get_map_tile_int_pos(cub, cub->player_px);
	player_float_map_pos = get_map_tile_px_pos(cub, cub->player_px);
	ray_dir.x = cosf(ray_angle);
    ray_dir.y = sinf(ray_angle);
    delta_dist.x = fabsf(1.0f / ray_dir.x);
    delta_dist.y = fabsf(1.0f / ray_dir.y);


    if (ray_dir.x < 0)
    {
        step.x = -1;
        side_dist.x = (player_float_map_pos.x - (float)map_pos.x) * delta_dist.x;
    }
    else
    {
        step.x = 1;
        side_dist.x = ((float)map_pos.x + 1.0f - player_float_map_pos.x) * delta_dist.x;
    }

    if (ray_dir.y < 0)
    {
        step.y = -1;
        side_dist.y = (player_float_map_pos.y - (float)map_pos.y) * delta_dist.y;
    }
    else
    {
        step.y = 1;
        side_dist.y = ((float)map_pos.y + 1.0f - player_float_map_pos.y) * delta_dist.y;
    }

    int is_horiz = 0; // vertical=0, horizontal=1

    //while (!hit_flag)
    while (!touches_wall(cub, map_pos.x, map_pos.y) && check_map_bounds_tiles(cub, map_pos.x, map_pos.y))
    {
        if (side_dist.x < side_dist.y)
        {
            side_dist.x += delta_dist.x;
            map_pos.x += step.x;
            is_horiz = 0;
        }
        else
        {
            side_dist.y += delta_dist.y;
            map_pos.y += step.y;
            is_horiz = 1;
        }
    }

	t_int_pos hit_tile;
	t_int_pos hit_pos;
	t_int_pos hit_map;
	hit_tile = map_pos;
	hit_pos = map_pos;

    float perp_dist;

    if (is_horiz == 0)
        perp_dist = (side_dist.x - delta_dist.x) * cub->tile_size;
    else
        perp_dist = (side_dist.y - delta_dist.y) * cub->tile_size;

    hit.side = is_horiz;
    hit.perp_dist = perp_dist;

    // exact hit position for textures
    if (is_horiz == 0)
        hit_pos.y = player_float_map_pos.y + (perp_dist / cub->tile_size) * ray_dir.y;
    else
        hit_pos.x = player_float_map_pos.x + (perp_dist / cub->tile_size) * ray_dir.x;

    return hit;
}



void	cast_rays(t_cub *cub) //todo block peaking??
{
	t_pos	ray_px;
	float	start_angle;
	int		i;

	start_angle = cub->player_angle - (cub->player_fov / 2);
	i = 0;
	while (i < cub->mlx_data.win_width)
	{
		ray_px = cub->player_px; //todo replace with dda
		set_last_ray_point(cub, start_angle, &ray_px); //todo replace with dda
		if (!DEBUG) //  3D version
			draw_vertical_slices(cub, i, &ray_px, start_angle);
		start_angle += cub->fraction_ray_angle;
		i++;
	}
}

int	render(t_cub *cub)
{
	//mlx_do_sync(cub->mlx);
	player_move(cub);
	clean_img(cub, 0x000000);
	if (DEBUG)
		draw_player_in_minimap(cub, 0x444444, 0xFFFFFF);
	cast_rays(cub);
	mlx_put_image_to_window(cub->mlx, cub->win, cub->img, 0, 0);
	return (0);
}

void	make_window(t_cub *cub)
{
	cub->win = mlx_new_window(cub->mlx, cub->mlx_data.win_width,
			cub->mlx_data.win_height, "cub3D");
	if (!cub->win)
		error_exit(cub, "Failed to create window\n", NULL);
	cub->img = mlx_new_image(cub->mlx, cub->mlx_data.win_width,
			cub->mlx_data.win_height);
	if (!cub->img)
		error_exit(cub, "Failed to create image\n", NULL);
	cub->pxl_arr = mlx_get_data_addr(
		cub->img, &cub->mlx_data.bits_per_pixel, &cub->mlx_data.line_length,
			&cub->mlx_data.endian);
	mlx_put_image_to_window(cub->mlx, cub->win, cub->img, 0, 0);
}

/*
CHECKING intersections (Permadi, 1996)

Note: remember the Cartesian coordinate is increasing downward (as in page 3),
	and any fractional values will be rounded down.

======Finding horizontal intersection ======

   A.x = Px + (Py-A.y)/tan(ALPHA);
   In the picture, (assume ALPHA is 60 degrees),
   A.x=96 + (224-191)/tan(60) = about 115;
   The grid coordinate of A.x is 115/64 = 1;

   So A is at grid (1,2) and we can check
   whether there is a wall on that grid.
   There is no wall on (1,2) so the ray will be
   extended to C.

2. Finding Ya
   If the ray is facing up
     Ya=-64;
   If the ray is facing down
     Ya=64;

3. Finding Xa
   Xa = 64/tan(60) = 36;

4. We can get the coordinate of C as follows:
   C.x=A.x+Xa = 115+36 = 151;
   C.y=A.y+Ya = 191-64 = 127;
   Convert this into grid coordinate by
   dividing each component with 64.
   The result is
   C.x = 151/64 = 2 (grid coordinate),
   C.y = 127/64 = 1 (grid coordinate)
   So the grid coordinate of C is (2, 1).
   (C programmer's note: Remember we always round down,
   this is especially true since
   you can use right shift by 8 to divide by 64).

5. Grid (2,1) is checked.
   Again, there is no wall, so the ray is extended
   to D.

6. We can get the coordinate of D as follows:
   D.x=C.x+Xa = 151+36 = 187;
   D.y=C.y+Ya = 127-64 = 63;
   Convert this into grid coordinate by
   dividing each component with 64.
   The result is
   D.x = 187/64 = 2 (grid coordinate),
   D.y = 63/64 = 0 (grid coordinate)
   So the grid coordinate of D is (2, 0).

6. Grid (2,0) is checked.
   There is a wall there, so the process stop.
*/
