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
	char *pixel;
	int i;
	int j;

	i = 0;
	while (i < cub->mlx_data.win_height)
	{
		j = 0;
		while (j < cub->mlx_data.win_width)
		{
			{
				pixel = cub->pxl_arr + (i * cub->mlx_data.line_length) + (j
						* cub->mlx_data.bytes_per_pixel);
				*(unsigned int *)pixel = color;
			}
			j++;
		}
		i++;
	}
}

/*
 *
* ray_dir	step
ray_dir.x < 0	go left
ray_dir.x > 0	go right
ray_dir.y < 0	go up
ray_dir.y > 0	go down
 */

void	set_step_direction_side_dist(t_hit *hit, float ray_angle)
{
	hit->ray_dir.x = cosf(ray_angle);
	hit->ray_dir.y = sinf(ray_angle);
	hit->delta_dist.x = fabsf(1.0f / hit->ray_dir.x);
	hit->delta_dist.y = fabsf(1.0f / hit->ray_dir.y);
	if (hit->ray_dir.x < 0)
	{
		hit->step.x = -1;
		hit->side_dist.x = (hit->player_float_map_pos.x - (float)hit->map_pos.x)
			* hit->delta_dist.x;
	}
	else
	{
		hit->step.x = 1;
		hit->side_dist.x = ((float)hit->map_pos.x + 1.0f
				- hit->player_float_map_pos.x) * hit->delta_dist.x;
	}
	if (hit->ray_dir.y < 0)
	{
		hit->step.y = -1;
		hit->side_dist.y = (hit->player_float_map_pos.y - (float)hit->map_pos.y)
			* hit->delta_dist.y;
	}
	else
	{
		hit->step.y = 1;
		hit->side_dist.y = ((float)hit->map_pos.y + 1.0f
				- hit->player_float_map_pos.y) * hit->delta_dist.y;
	}
}

void	find_hit(t_cub *cub, t_hit *hit)
{
	hit->is_horiz = 0;
	while (!touches_wall(cub, hit->map_pos.x, hit->map_pos.y)
		&& check_map_bounds_tiles(cub, hit->map_pos.x, hit->map_pos.y))
	{
		if (hit->side_dist.x < hit->side_dist.y)
		{
			hit->side_dist.x += hit->delta_dist.x;
			hit->map_pos.x += hit->step.x;
			hit->is_horiz = 0;
		}
		else
		{
			hit->side_dist.y += hit->delta_dist.y;
			hit->map_pos.y += hit->step.y;
			hit->is_horiz = 1;
		}
		if (DEBUG)
			try_put_pixel(cub, hit->map_pos.x, hit->map_pos.x, 0xFF0000);
	}
}

t_hit	dda_ray(t_cub *cub, float ray_angle)
{
	t_hit	hit;

	hit.map_pos = get_map_tile_int_pos(cub, cub->player_px);
	hit.player_float_map_pos = get_map_tile_px_pos(cub, cub->player_px);
	set_step_direction_side_dist(&hit, ray_angle);
	find_hit(cub, &hit);
	hit.hit_point.x = (float)hit.map_pos.x;
	hit.hit_point.y = (float)hit.map_pos.y;
	if (hit.is_horiz == 0)
	{
		hit.perp_dist = (hit.side_dist.x - hit.delta_dist.x) * cub->tile_size;
		hit.hit_point.y = hit.player_float_map_pos.y + (hit.perp_dist
				/ cub->tile_size) * hit.ray_dir.y;
		hit.wall_x = hit.hit_point.y - floorf(hit.hit_point.y);
	}
	else
	{
		hit.perp_dist = (hit.side_dist.y - hit.delta_dist.y) * cub->tile_size;
		hit.hit_point.x = hit.player_float_map_pos.x + (hit.perp_dist
				/ cub->tile_size) * hit.ray_dir.x;
		hit.wall_x = hit.hit_point.x - floorf(hit.hit_point.x);
	}
	return (hit);
}

void	set_rel_x_tile_pos(t_hit *hit)
{
	hit->rel_pos_x = (int)(hit->wall_x * hit->tex.width);
	if (!hit->is_horiz && hit->ray_dir.x < 0)
		hit->rel_pos_x = hit->tex.width - hit->rel_pos_x - 1;
	if (hit->is_horiz && hit->ray_dir.y > 0)
		hit->rel_pos_x = hit->tex.width - hit->rel_pos_x - 1;
	hit->rel_pos_x = (int)fmin(fmax(hit->rel_pos_x, 0), hit->tex.width - 1);
}


/*
	- set up values and clip as necessary for one vertical slice of the screen (determine wall dims) and determine
 * texture ratio
 * - correct distance to avoid fisheye,
	and clip to avoid instability in close wall proximity (dist of actual 0 should
 * not be possible --> division by 0)
 * returns: populated slice
 */
t_w_slice	set_up_wall_slice(t_cub *cub, t_hit *hit, float start_angle)
{
	t_w_slice	slice;

	slice.corrected_dist = hit->perp_dist * cosf(start_angle
			- cub->player_angle);
	if (slice.corrected_dist < 0.001f)
		slice.corrected_dist = 0.001f;
	slice.wall_height = ((float)cub->tile_size / slice.corrected_dist)
		* cub->screen_dist;
	slice.wall_start_y = (cub->mlx_data.win_height - (int)slice.wall_height)
		/ 2;
	slice.wall_end_y = slice.wall_start_y + (int)slice.wall_height;
	slice.clipped = 0;
	if (slice.wall_start_y < 0)
	{
		slice.clipped = -slice.wall_start_y;
		slice.wall_start_y = 0;
	}
	slice.tex_step = (float)hit->tex.height / slice.wall_height;
	slice.tex_pos = slice.clipped * slice.tex_step;
	slice.tex_px_wo_y = hit->tex.pxl_arr + hit->rel_pos_x * hit->tex.bytes_per_pixel;
	return (slice);
}

void	draw_vertical_slices(t_cub *cub, int i, t_hit *hit, float start_angle)
{
	t_w_slice	slice;
	int			y;
	int			tex_y;
	int			wall_px;

	slice = set_up_wall_slice(cub, hit, start_angle);
	y = 0;
	while (y < cub->mlx_data.win_height)
	{
		if (y < slice.wall_start_y)
			try_put_pixel(cub, i, y, cub->col->ceil);
		else if (y >= slice.wall_start_y && y < slice.wall_end_y)
		{
			tex_y = (int)slice.tex_pos;
			slice.tex_pos += slice.tex_step;
			wall_px = *(int *)(slice.tex_px_wo_y + tex_y * hit->tex.line_len);
			try_put_pixel(cub, i, y, wall_px);
		}
		else
			try_put_pixel(cub, i, y, cub->col->floor);
		y++;
	}
}

void	cast_rays(t_cub *cub) //todo block peaking??
{
	t_hit		hit;
	float		start_angle;
	int			i;

	start_angle = cub->player_angle - (cub->player_fov / 2);
	i = 0;
	while (i < cub->mlx_data.win_width)
	{
		hit = dda_ray(cub, start_angle);
		hit.tex = select_texture(cub, hit.ray_dir, hit.is_horiz);
		set_rel_x_tile_pos(&hit);
		if (!DEBUG) //  3D version
			draw_vertical_slices(cub, i, &hit, start_angle);
		start_angle += cub->fraction_ray_angle;
		i++;
	}
}

/* non dda version
void	cast_rays(t_cub *cub) //todo block peaking??
{
	t_pos		ray_px;
	float		start_angle;
	int			i;

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
}*/

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
	cub->mlx_data.bytes_per_pixel = cub->mlx_data.bits_per_pixel / 8;
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
