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

/*
//todo also consider player width?
int check_walkable_pos(t_cub *cub, float new_x, float new_y)
{
   int mx = (int)new_x;
   int my = (int)new_y;

   // Check map bounds
   if (mx < 0 || my < 0 || mx >= cub->map->width || my >= cub->map->height)
   {
      printf("!!! out of map bounds for:%f, %f max width, height:%d, %d\n", cub->player_pos.x, cub->player_pos.y, cub->mlx_data.win_width, cub->mlx_data.win_height);
      return 0;
   }
   if (cub->map->grid[my][mx] != '1')    //Check walls
      return 1;
   printf("!!! running into wall at :%f, %f max width, height:%d, %d\n", cub->player_pos.x, cub->player_pos.y, cub->mlx_data.win_width, cub->mlx_data.win_height);
   return 0;

}*/
int check_walkable_pos(t_cub *cub, float new_x, float new_y, float radius)
{
   int x0 = (int)(new_x - radius);
   int x1 = (int)(new_x + radius);
   int y0 = (int)(new_y - radius);
   int y1 = (int)(new_y + radius);

   for (int y = y0; y <= y1; y++)
   {
      for (int x = x0; x <= x1; x++)
      {
         if (x < 0 || y < 0 || x >= cub->map->width || y >= cub->map->height)
            return 0; // out of bounds
         if (cub->map->grid[y][x] == '1')
            return 0; // wall collision
      }
   }
   return 1;
}

int check_map_bounds_tiles(t_cub *cub, int x_tile, int y_tile)
{
   if (x_tile < 0 || x_tile >= cub->map->width ||
       y_tile < 0 || y_tile >= cub->map->height)
      return 0;
   return 1;
}

int check_screen_bounds(t_cub *cub, int cx, int cy)
{
    if (cx <0 || cy < 0 ||
      cx >= cub->mlx_data.win_width
      || cy >= cub->mlx_data.win_height)
      return 0;
   return 1;
}
void draw_cube(t_cub *cub, int x_start, int y_start, int color)
{
   int     i;
   int     j;
   int     x;
   int     y;
   char    *pixel;

   i = 0;
   while (i < cub->mlx_data.tile_size)
   {
      j = 0;
      while (j < cub->mlx_data.tile_size)
      {
         x = x_start + i;
         y = y_start + j;

         if (check_screen_bounds(cub, x, y))
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

void clean_img(t_cub *cub, int color) //todo replace one color with true map pixel colors
{
   int     i;
   int     j;
   char    *pixel;

   i = 0;
   while (i < cub->mlx_data.win_height)
   {
      j = 0;
      while (j < cub->mlx_data.win_width)
      {
         {
            pixel = cub->pxl_arr
                + (i * cub->mlx_data.line_length)
                + (j * (cub->mlx_data.bits_per_pixel / 8));
            *(unsigned int *)pixel = color;
         }
         j++;
      }
      i++;
   }
}

void draw_map(t_cub *cub, int color) //todo replace one color with true map pixel colors
{
   int     i;
   int     j;

   i = 0;
   while (cub->map->grid[i])
   {
      j = 0;
      while (cub->map->grid[i][j])
      {
         if (cub->map->grid[i][j] == '1')
            draw_cube(cub, j*cub->mlx_data.tile_size, i*cub->mlx_data.tile_size, color);
         if (cub->map->grid[i][j] == '0')
            draw_cube(cub, j*cub->mlx_data.tile_size, i*cub->mlx_data.tile_size, 0x000000);
         if (cub->map->grid[i][j] == ' ')
            draw_cube(cub, j*cub->mlx_data.tile_size, i*cub->mlx_data.tile_size, 0x111111);
         j++;
      }
      i++;
   }
}



int render(t_cub *cub)
{
   //mlx_do_sync(cub->mlx);
   //printf("player pos:%f, %f\n", cub->player_pos.x, cub->player_pos.y);
   //printf("render loop called\n");
   player_move(cub);
   clean_img(cub, 0x000000);
   //todo ensure rendering is not at 0,0 with half the square off screen
   //draw_cube(cub, cub->player_pos.x*CUBE_SIZE, (int)cub->player_pos.y*CUBE_SIZE, 0xFFFFFF);
   draw_map(cub, 0x444444);
   //draw_cube(cub, cub->player_pos.x*cub->map->tile_size, (int)cub->player_pos.y*cub->map->tile_size, 0xFFFFFF);
   draw_cube(cub,
      (cub->player_tile.x*cub->mlx_data.tile_size)-(cub->mlx_data.tile_size/2),
      (cub->player_tile.y*cub->mlx_data.tile_size)-(cub->mlx_data.tile_size/2), 0xFFFFFF);

   //cast_rays(cub);
	float fov = PI / 3; //60 deg
   float fraction = fov / cub->mlx_data.win_width;
   float start_angle = cub->player_angle - (fov / 2);
   int i = 0;
   while(i < cub->mlx_data.win_width)
   {
      draw_ray(cub, start_angle, i);
      start_angle += fraction;
      i++;
   }
   mlx_put_image_to_window(cub->mlx, cub->win, cub->img, 0, 0);
   return(0);
}

void	make_window(t_cub *cub)
{
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
