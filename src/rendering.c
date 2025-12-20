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

// overwrite every pixel with one color
void	clean_img(t_cub *cub, int color)
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
						* cub->mlx_data.bytes_per_pixel);
				*(unsigned int *)pixel = color;
			}
			j++;
		}
		i++;
	}
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

void	cast_rays(t_cub *cub)
{
	t_hit hit;
	float start_angle;
	int i;

	start_angle = cub->player_angle - (cub->player_fov / 2);
	i = 0;
	while (i < cub->mlx_data.win_width)
	{
		hit = cast_dda_ray(cub, start_angle);
		hit.tex = select_texture(cub, hit.ray_dir, hit.is_horiz);
		set_rel_x_tile_pos(&hit);
		draw_vertical_slices(cub, i, &hit, start_angle);
		start_angle += cub->fraction_ray_angle;
		i++;
	}
}

int	render(t_cub *cub)
{
	player_move(cub);
	clean_img(cub, 0x000000);
	cast_rays(cub);
	draw_minimap(cub);
	mlx_put_image_to_window(cub->mlx, cub->win, cub->img, 0, 0);
	return (0);
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
