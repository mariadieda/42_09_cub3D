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

t_int_pos	get_map_tile_int_pos(t_cub *cub, t_pos px_pos)
{
	t_int_pos	map_pos;

	map_pos.x = (int)px_pos.x / cub->tile_size;
	map_pos.y = (int)px_pos.y / cub->tile_size;
	return (map_pos);
}

t_pos	get_map_tile_px_pos(t_cub *cub, t_pos px_pos)
{
	t_pos	map_pos;

	map_pos.x = px_pos.x / cub->tile_size;
	map_pos.y = px_pos.y / cub->tile_size;
	return (map_pos);
}
