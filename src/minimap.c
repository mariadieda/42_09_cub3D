/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:15:54 by mdiederi          #+#    #+#             */
/*   Updated: 2025/12/16 10:15:57 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../inc/cub3d.h"

void	put_line_pixels(t_cub *cub, t_line *line, int color)
{
	float	x_cord;
	float	y_cord;
	int	i;

	i = 0;
	while (i <= (int)line->line_len)
	{
		x_cord = (float)(line->start_point.x + (i / line->line_len)
				* line->x_diff);
		y_cord = (float)(line->start_point.y + (i / line->line_len)
				* line->y_diff);
		if (x_cord >= 0 && x_cord < (float)cub->mlx_data.win_width && y_cord >= 0
			&& y_cord < (float)cub->mlx_data.win_height)
		{
			try_put_pixel(cub, x_cord, y_cord, color);
		}
		i++;
	}
}

void	draw_line(t_cub *cub, t_pos start_pos_px, t_pos end_pos_px, int color)
{
	t_line	line;

	ft_memset(&line, 0, sizeof(t_line));
	line.start_point = start_pos_px;
	line.end_point = end_pos_px;
	line.x_diff = line.end_point.x - line.start_point.x;
	line.y_diff = line.end_point.y - line.start_point.y;
	line.line_len = sqrt(line.x_diff * line.x_diff + line.y_diff * line.y_diff);
	if (line.line_len > 0)
		put_line_pixels(cub, &line, color);
}


void	draw_tile(t_cub *cub, t_minimap mm, t_pos map_px, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < mm.tile_px)
	{
		j = 0;
		while (j < mm.tile_px)
		{
			try_put_pixel(
				cub,
				map_px.x + i,
				map_px.y + j,
				color
			);
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
			start_px.x = (float)(mm.offset.x + j * mm.tile_px);
			start_px.y = (float)(mm.offset.y + i * mm.tile_px);
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
	left.x = player_pos.x + cosf(angle + M_PI * 0.75f) * scale * 0.7f;
	left.y = player_pos.y + sinf(angle + M_PI * 0.75f) * scale * 0.7f;
	right.x = player_pos.x + cosf(angle - M_PI * 0.75f) * scale * 0.7f;
	right.y = player_pos.y + sinf(angle - M_PI * 0.75f) * scale * 0.7f;
	draw_line(cub, tip, left, mm.player_color);
	draw_line(cub, left, right, mm.player_color);
	draw_line(cub, right, tip, mm.player_color);
}

void draw_minimap(t_cub *cub)
{
	t_pos player_mm;
	t_minimap minimap;

	minimap.offset.x = 10;
	minimap.offset.y = 10;
	minimap.scale = fminf(
		(cub->mlx_data.win_width  * 0.25f) / (cub->map->width * cub->tile_size),
		(cub->mlx_data.win_height * 0.25f) / (cub->map->height * cub->tile_size));
	if (minimap.scale < 0.01f)
		minimap.scale = 0.01f;
	minimap.tile_px = (int)(cub->tile_size * minimap.scale);
	if (minimap.tile_px < 1)
		minimap.tile_px = 1;
	minimap.player_color = 0xFF4444;
	minimap.wall_color = 0x000000;
	minimap.walk_color = 0x444444;
	minimap.space_color = 0x222222;
	draw_map(cub, minimap);
	player_mm.x = minimap.offset.x
		+ (cub->player_px.x / cub->tile_size) * minimap.tile_px;
	player_mm.y = minimap.offset.y
		+ (cub->player_px.y / cub->tile_size) * minimap.tile_px;
	draw_player_triangle(cub, player_mm, cub->player_angle,minimap.tile_px * 0.6f, minimap);
}

//alternatively draw player cub
//draw_cube(cub, (cub->player_px.x)-(float)(cub->tile_size*0.6/2),
// (cub->player_px.y)-(float)(cub->tile_size*0.6/2), 0.6,0xFFFFFF);
