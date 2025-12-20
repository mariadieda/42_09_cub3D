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


void	draw_tile(t_cub *cub, t_minimap mm, t_pos map_px, int color)
{
	int		i;
	int		j;
	float	x;
	float	y;

	i = 0;
	while (i < (int)(cub->tile_size * mm.scale))
	{
		j = 0;
		while (j < (int)(cub->tile_size * mm.scale))
		{
			x = mm.offset.x + (map_px.x * mm.scale) + (float)i;
			y = mm.offset.y + (map_px.y * mm.scale) + (float)j;
			try_put_pixel(cub, x, y, color);
			j++;
		}
		i++;
	}
}

void	draw_map(t_cub *cub, t_minimap mm)
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
			start_px.x = (float)(j * cub->tile_size);
			start_px.y = (float)(i * cub->tile_size);
			if (cub->map->grid[i][j] == '1')
				draw_tile(cub, mm, start_px, mm.wall_color);
			if (cub->map->grid[i][j] == '0')
				draw_tile(cub, mm, start_px, mm.walk_color);
			if (cub->map->grid[i][j] == ' ')
				draw_tile(cub, mm, start_px, mm.space_color);
			j++;
		}
		i++;
	}
}
/*
void draw_map_minimap(t_cub *cub, t_pos offset, float scale)
{
	t_pos map_px;

	for (int y = 0; cub->map->grid[y]; y++)
	{
		for (int x = 0; cub->map->grid[y][x]; x++)
		{
			map_px.x = x * cub->tile_size;
			map_px.y = y * cub->tile_size;

			if (cub->map->grid[y][x] == '1')
				draw_tile_minimap(cub, map_px, offset, scale, 0x666666);
			else
				draw_tile_minimap(cub, map_px, offset, scale, 0x111111);
		}
	}
}
*/

// add size to player pos to find tip, [-size/2 forward
//	+ size/2 left] to find left
// [-size/2 forward - size/2 left] to find right
void	draw_player_triangle(t_cub *cub, t_pos player_pos, float angle, float scale, t_minimap mm)
{
	t_pos	tip;
	t_pos	left;
	t_pos	right;

	tip.x = player_pos.x + cosf(angle) * scale;
	tip.y = player_pos.y + sinf(angle) * scale;
	left.x = player_pos.x + cosf(angle + PI * 0.75f) * scale * 0.7f;
	left.y = player_pos.y + sinf(angle + PI * 0.75f) * scale * 0.7f;
	right.x = player_pos.x + cosf(angle - PI * 0.75f) * scale * 0.7f;
	right.y = player_pos.y + sinf(angle - PI * 0.75f) * scale * 0.7f;
	//draw_line(cub, player_pos, tip, color);
	draw_line(cub, tip, left, mm.player_color);
	draw_line(cub, left, right, mm.player_color);
	draw_line(cub, right, tip, mm.player_color);
}





void draw_minimap(t_cub *cub)
{
	t_pos player_mm;
	t_minimap minimap;
	float player_size;

	minimap.offset.x = 10;
	minimap.offset.y = 10;
	minimap.scale = fminf(
		(cub->mlx_data.win_width  * 0.25f) / (cub->map->width * cub->tile_size),
		(cub->mlx_data.win_height * 0.25f) / (cub->map->height * cub->tile_size));
	if (minimap.scale < 0.01f)
		minimap.scale = 0.01f;
	minimap.player_color = 0x444444;
	minimap.wall_color = 0x606060;
	minimap.walk_color = 0xFFFFFF;
	minimap.space_color = 0x111111;
	draw_map(cub, minimap);
	player_mm.x = minimap.offset.x + cub->player_px.x * minimap.scale;
	player_mm.y = minimap.offset.y + cub->player_px.y * minimap.scale;
	player_size = cub->tile_size * 0.6f * minimap.scale;
	draw_player_triangle(cub, player_mm, cub->player_angle,player_size, minimap);
}

//alternatively draw player cub
//draw_cube(cub, (cub->player_px.x)-(float)(cub->tile_size*0.6/2),
// (cub->player_px.y)-(float)(cub->tile_size*0.6/2), 0.6,0xFFFFFF);
