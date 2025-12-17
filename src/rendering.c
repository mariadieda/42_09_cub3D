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

void	cast_rays(t_cub *cub) //todo block peaking??
{
	t_pos	ray_px;
	float	start_angle;
	int		i;

	start_angle = cub->player_angle - (cub->player_fov / 2);
	i = 0;
	while (i < cub->mlx_data.win_width)
	{
		ray_px = cub->player_px;
		set_last_ray_point(cub, start_angle, &ray_px);
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
