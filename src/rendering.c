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
#include"../inc/cub3d.h"

int check_px_bounds(t_cub *cub, int cx, int cy)
{
   if (cx <0 || cy < 0 ||
      cx >= cub->mlx_data.win_width
      || cy >= cub->mlx_data.win_height)
      return 0;
   return 1;
}
void draw_cube_centered(t_cub *cub, int cx, int cy, int color)
{
   int     i;
   int     j;
   int     x;
   int     y;
   int     x_start = cx - CUBE_SIZE / 2;
   int     y_start = cy - CUBE_SIZE / 2;
   char    *pixel;

   i = 0;
   while (i < CUBE_SIZE)
   {
      j = 0;
      while (j < CUBE_SIZE)
      {
         x = x_start + i;
         y = y_start + j;

         if (check_px_bounds(cub, x, y))
         {
            pixel = cub->pxl_arr
                + (y * cub->mlx_data.line_length)
                + (x * (cub->mlx_data.bits_per_pixel / 8));

            *(unsigned int *)pixel = color;
         }
         j++;
      }
      i++;
   }
}

int render(t_cub *cub)
{
   printf("player pos:%f, %f", cub->player_pos.x, cub->player_pos.y);
   player_move(cub);
   draw_cube_centered(cub, (int)cub->player_pos.x, (int)cub->player_pos.y, 0xFFFFFF);
   //game_update_and_render(cub);
   //clear_image(cub->img);
   //cast_rays(cub);
   mlx_put_image_to_window(cub->mlx, cub->win, cub->img, 0, 0);
   return(0);
}

void	make_window(t_cub *cub)
{
   cub->mlx = mlx_init();
   if (!cub->mlx)
      error_exit(cub, "Failed to initialize MLX\n", NULL);

   mlx_get_screen_size(cub->mlx, &cub->mlx_data.screen_width,
      &cub->mlx_data.screen_height);
   cub->mlx_data.win_width = (int)round(cub->mlx_data.screen_width * 0.7);
   cub->mlx_data.win_height = (int)round(cub->mlx_data.screen_height * 0.7);
   
   cub->win = mlx_new_window(cub->mlx, cub->mlx_data.win_width,
         cub->mlx_data.win_height, "cub3D");
   if (!cub->win)
      error_exit(cub, "Failed to create window\n", NULL);
   cub->img = mlx_new_image(cub->mlx, cub->mlx_data.win_width, cub->mlx_data.win_height);
   if (!cub->img)
      error_exit(cub, "Failed to create image\n", NULL);
   cub->pxl_arr = mlx_get_data_addr(
      cub->img,
      &cub->mlx_data.bits_per_pixel,
      &cub->mlx_data.line_length,
      &cub->mlx_data.endian
   );
   mlx_put_image_to_window(cub->mlx, cub->win, cub->img, 0, 0);
}


/*
CHECKING intersections (Permadi, 1996)

Note: remember the Cartesian coordinate is increasing downward (as in page 3), and any fractional values will be rounded down.

======Finding horizontal intersection ======
1. Finding the coordinate of A.
   If the ray is facing up
     A.y = rounded_down(Py/64) * (64) - 1;
   If the ray is facing down
     A.y = rounded_down(Py/64) * (64) + 64;

   (In the picture, the ray is facing up, so we use
   the first formula.
   A.y=rounded_down(224/64) * (64) - 1 = 191;
   Now at this point, we can find out the grid
   coordinate of y.
   However, we must decide whether A is part of
   the block above the line,
   or the block below the line.
   Here, we chose to make A part of the block
   above the line, that is why we subtract 1 from A.y.
   So the grid coordinate of A.y is 191/64 = 2;

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
