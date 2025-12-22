/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 16:50:48 by mdiederi          #+#    #+#             */
/*   Updated: 2025/12/20 16:50:52 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../inc/cub3d.h"

/*
* ray_dir	step
ray_dir.x < 0	go left
ray_dir.x > 0	go right
ray_dir.y < 0	go up
ray_dir.y > 0	go down
 */
void	set_step_direction_side_dist(t_hit *hit)
{
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
	}
	hit->hit_point.x = (float)hit->map_pos.x;
	hit->hit_point.y = (float)hit->map_pos.y;
}

t_hit	cast_dda_ray(t_cub *cub, float ray_angle)
{
	t_hit	hit;

	hit.map_pos = get_map_tile_int_pos(cub, cub->player_px);
	hit.player_float_map_pos = get_map_tile_px_pos(cub, cub->player_px);
	hit.ray_dir.x = cosf(ray_angle);
	hit.ray_dir.y = sinf(ray_angle);
	hit.delta_dist.x = fabsf(1.0f / hit.ray_dir.x);
	hit.delta_dist.y = fabsf(1.0f / hit.ray_dir.y);
	set_step_direction_side_dist(&hit);
	find_hit(cub, &hit);
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
- set up values and clip as necessary for one vertical slice of the screen
	(determine wall dims) and determine texture ratio
- correct distance to avoid fisheye,
- clip to avoid instability in close wall proximity (dist of actual 0 should
	not be possible --> division by 0)
- returns: populated slice
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
	slice.tex_px_wo_y = hit->tex.pxl_arr + hit->rel_pos_x
		* hit->tex.bytes_per_pixel;
	return (slice);
}
