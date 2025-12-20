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
int	check_walkable_pos(t_cub *cub, float new_x_px, float new_y_px)
{
    float		radius;
    int			left;
    int			top;
    int			right;
    int			bottom;
    int			y;
    int			x;

    radius = 0.3f * cub->tile_size;
    left = (int)floorf((new_x_px - radius - 0.0001f) / cub->tile_size);
    right = (int)floorf((new_x_px + radius - 0.0001f) / cub->tile_size);
    top = (int)floorf((new_y_px - radius - 0.0001f) / cub->tile_size);
    bottom = (int)floorf((new_y_px + radius - 0.0001f) / cub->tile_size);
    y = top;
    while (y <= bottom)
    {
        x = left;
        while (x <= right)
        {
            if (x < 0 || y < 0 || x >= cub->map->width || y >= cub->map->height)
                return (0);
            if (cub->map->grid[y][x] == '1')
                return (0);
            x++;
        }
        y++;
    }
    return (1);
}

int	check_map_bounds_tiles(t_cub *cub, int x_tile, int y_tile)
{
    if (x_tile < 0 || x_tile >= cub->map->width
        || y_tile < 0 || y_tile >= cub->map->height)
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