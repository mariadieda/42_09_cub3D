/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision_handling.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:04:07 by mdiederi          #+#    #+#             */
/*   Updated: 2025/12/17 17:04:11 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../inc/cub3d.h"

// models player as a circle/spere with radius of 30% size of a tile
// add small number to prevent tile boundary pos edge cases
int	calc_player_dims(t_cub *cub, float new_px_coord, int neg_radius)
{
	float	radius;
	int		pos;

	radius = 0.3f * cub->tile_size;
	if (neg_radius)
		radius = -radius;
	pos = (int)floorf((new_px_coord + radius - 0.0001f) / cub->tile_size);
	return (pos);
}

int	check_walkable_pos(t_cub *cub, float new_x_px, float new_y_px)
{
	int			left;
	int			top;
	int			right;
	int			bottom;
	t_int_pos	pos;

	left = calc_player_dims(cub, new_x_px, 1);
	right = calc_player_dims(cub, new_x_px, 0);
	top = calc_player_dims(cub, new_y_px, 1);
	bottom = calc_player_dims(cub, new_y_px, 0);
	pos.y = top;
	while (pos.y <= bottom)
	{
		pos.x = left;
		while (pos.x <= right)
		{
			if (!check_map_bounds_tiles(cub, pos.x, pos.y)
				|| cub->map->grid[pos.y][pos.x] == '1')
				return (0);
			pos.x++;
		}
		pos.y++;
	}
	return (1);
}

int	check_map_bounds_tiles(t_cub *cub, int x_tile, int y_tile)
{
	if (x_tile < 0 || x_tile >= cub->map->width || y_tile < 0
		|| y_tile >= cub->map->height)
		return (0);
	return (1);
}

int	check_screen_bounds_px(t_cub *cub, float x_px, float y_px)
{
	int	x;
	int	y;

	x = (int)x_px;
	y = (int)y_px;
	if (x < 0 || y < 0 || x >= cub->mlx_data.win_width
		|| y >= cub->mlx_data.win_height)
		return (0);
	return (1);
}

int	touches_wall(t_cub *cub, int x_tile, int y_tile)
{
	if (cub->map->grid[y_tile][x_tile] == '1')
		return (1);
	return (0);
}
