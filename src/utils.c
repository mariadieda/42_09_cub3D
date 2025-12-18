/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:17:28 by mdiederi          #+#    #+#             */
/*   Updated: 2025/12/16 10:17:31 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

t_int_pos get_map_tile_int_pos(t_cub *cub, t_pos px_pos)
{
	t_int_pos map_pos;
	map_pos.x = (int)px_pos.x / cub->tile_size;
	map_pos.y = (int)px_pos.y / cub->tile_size;

	return (map_pos);
}

t_pos get_map_tile_px_pos(t_cub *cub, t_pos px_pos)
{
	t_pos map_pos;
	map_pos.x = px_pos.x / cub->tile_size;
	map_pos.y = px_pos.y / cub->tile_size;

	return (map_pos);
}

int	ft_fmin(const double x, const double y)
{
	int	min;

	if (x <= y)
		min = (int)x;
	else
		min = (int)y;
	return (min);
}

void	print_map(t_cub *cub)
{
	int	i;

	i = 0;
	while (cub->map->grid[i])
	{
		printf("\n%s\n\n", cub->map->grid[i]);
		i++;
	}
}
