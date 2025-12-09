#include "../inc/cub3d.h"

void draw_cube(t_cub *cub, float x_start_px, float y_start_px, int color)
{
    int     i;
    int     j;
    int     x;
    int     y;
    char    *pixel;

    i = 0;
    while (i < cub->tile_size)
    {
        j = 0;
        while (j < cub->tile_size)
        {
            x = x_start_px + i;
            y = y_start_px + j;

            if (check_screen_bounds_px(cub, x, y))
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
                draw_cube(cub, j*cub->tile_size, i*cub->tile_size, color);
            if (cub->map->grid[i][j] == '0')
                draw_cube(cub, j*cub->tile_size, i*cub->tile_size, 0x000000);
            if (cub->map->grid[i][j] == ' ')
                draw_cube(cub, j*cub->tile_size, i*cub->tile_size, 0x111111);
            j++;
        }
        i++;
    }
}