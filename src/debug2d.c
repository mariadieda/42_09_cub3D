/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug2d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:15:54 by mdiederi          #+#    #+#             */
/*   Updated: 2025/12/16 10:15:57 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	draw_tile(t_cub *cub, t_pos start_px, float size_factor, int color)
{
	char	*pixel;
	int		i;
	int		j;
	int		x;
	int		y;

	i = 0;
	while (i < (int)(cub->tile_size * size_factor))
	{
		j = 0;
		while (j < (int)(cub->tile_size * size_factor))
		{
			x = start_px.x + i;
			y = start_px.y + j;
			if (check_screen_bounds_px(cub, x, y))
			{
				pixel = cub->pxl_arr + (y * cub->mlx_data.line_length) + (x
						* (cub->mlx_data.bits_per_pixel / 8));
				*(unsigned int *)pixel = color;
			}
			j++;
		}
		i++;
	}
}

/* add size to player pos to find tip, [-size/2 forward
	+ size/2 left] to find left
 * [-size/2 forward - size/2 left] to find right */
void	draw_player_triangle(t_cub *cub, float angle, float size, int color)
{
	t_pos	tip;
	t_pos	left;
	t_pos	right;

	tip.x = cub->player_px.x + cosf(angle) * size;
	tip.y = cub->player_px.y + sinf(angle) * size;
	left.x = cub->player_px.x + cosf(angle + PI * 0.75f) * size * 0.7f;
	left.y = cub->player_px.y + sinf(angle + PI * 0.75f) * size * 0.7f;
	right.x = cub->player_px.x + cosf(angle - PI * 0.75f) * size * 0.7f;
	right.y = cub->player_px.y + sinf(angle - PI * 0.75f) * size * 0.7f;
	draw_line(cub, cub->player_px, tip, color);
	draw_line(cub, tip, left, color);
	draw_line(cub, left, right, color);
	draw_line(cub, right, tip, color);
}

void	draw_player_in_minimap(t_cub *cub, int wall_color, int triangle_color)
{
	draw_map(cub, wall_color);
	draw_player_triangle(cub, cub->player_angle, cub->tile_size * 0.6,
		triangle_color);
}
//alternatively draw player cub
/*draw_cube(cub, (cub->player_px.x)-(float)(cub->tile_size*0.6/2),
		(cub->player_px.y)-(float)(cub->tile_size*0.6/2), 0.6,0xFFFFFF);*/

//todo replace one color with true map pixel colors
void	draw_map(t_cub *cub, int color)
{
	t_pos	start_px;
	int		i;
	int		j;

	i = 0;
	while (cub->map->grid[i])
	{
		j = 0;
		while (cub->map->grid[i][j])
		{
			start_px.x = j * cub->tile_size;
			start_px.y = i * cub->tile_size;
			if (cub->map->grid[i][j] == '1')
				draw_tile(cub, start_px, 1, color);
			if (cub->map->grid[i][j] == '0')
				draw_tile(cub, start_px, 1, 0x000000);
			if (cub->map->grid[i][j] == ' ')
				draw_tile(cub, start_px, 1, 0x111111);
			j++;
		}
		i++;
	}
}
