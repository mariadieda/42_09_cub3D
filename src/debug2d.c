#include "../inc/cub3d.h"

void draw_tile(t_cub *cub, float x_start_px, float y_start_px, float size_factor, int color)
{
    int     i;
    int     j;
    int     x;
    int     y;
    char    *pixel;

    i = 0;
    while (i < (int)(cub->tile_size * size_factor))
    {
        j = 0;
        while (j < (int)(cub->tile_size * size_factor))
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


void draw_player_in_minimap(t_cub *cub, int wall_color, int triangle_color)
{
    draw_map(cub, wall_color);
    draw_player_triangle(cub, cub->player_angle, cub->tile_size*0.6 , triangle_color);
    //alternatively draw player cub
    /*draw_cube(cub, (cub->player_px.x)-(float)(cub->tile_size*0.6/2), (cub->player_px.y)-(float)(cub->tile_size*0.6/2), 0.6,0xFFFFFF);*/

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
                draw_tile(cub, j*cub->tile_size, i*cub->tile_size, 1, color);
            if (cub->map->grid[i][j] == '0')
                draw_tile(cub, j*cub->tile_size, i*cub->tile_size, 1, 0x000000);
            if (cub->map->grid[i][j] == ' ')
                draw_tile(cub, j*cub->tile_size, i*cub->tile_size, 1, 0x111111);
            j++;
        }
        i++;
    }
}