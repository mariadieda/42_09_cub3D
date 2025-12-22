/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <apavlopo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:17:44 by mdiederi          #+#    #+#             */
/*   Updated: 2025/12/22 11:13:32 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	pad_map(t_cub *cub)
{
	int	y;
	int	old_width;
	int	add_size;

	add_size = 0;
	y = 0;
	while (y < cub->map->height)
	{
		old_width = ft_strlen(cub->map->grid[y]);
		add_size = cub->map->width - old_width + 1;
		cub->map->grid[y] = ft_realloc(cub->map->grid[y], old_width, (old_width
					+ add_size));
		while (old_width < cub->map->width)
		{
			cub->map->grid[y][old_width] = ' ';
			old_width++;
		}
		cub->map->grid[y][cub->map->width] = '\0';
		y++;
	}
}

void	allocate_map(t_cub *cub)
{
	cub->map = ft_calloc(1, sizeof(t_map));
	if (!cub->map)
		error_exit(cub, "Malloc failed\n", NULL);
	cub->map->height = 0;
	cub->map->width = 0;
	cub->map->capacity = 8;
	cub->map->grid = malloc(sizeof(char *) * (cub->map->capacity + 1));
	if (!cub->map->grid)
		error_exit(cub, "Malloc failed for map grid\n", NULL);
}

int	check_tile(int y, int x, char **grid, t_cub *cub)
{
	if ((y == 0) || (y == cub->map->height - 1) || x == 0
		|| (x == cub->map->width - 1))
	{
		if (grid[y][x] == '0' || is_player(grid[y][x]))
			return (0);
	}
	else if (grid[y][x] == '0' || is_player(grid[y][x]))
	{
		if ((grid[y - 1][x] != '1' && grid[y - 1][x] != '0'
				&& !is_player(grid[y - 1][x])))
			return (0);
		if ((grid[y][x - 1] != '1' && grid[y][x - 1] != '0'
				&& !is_player(grid[y][x - 1])))
			return (0);
		if ((grid[y + 1][x] != '1' && grid[y + 1][x] != '0'
				&& !is_player(grid[y + 1][x])))
			return (0);
		if ((grid[y][x + 1] != '1' && grid[y][x + 1] != '0'
				&& !is_player(grid[y][x + 1])))
			return (0);
	}
	return (1);
}

int	check_map(t_cub *cub)
{
	int		y;
	int		x;
	int		h;
	int		w;
	char	**grid;

	grid = cub->map->grid;
	y = 0;
	if (cub->map->width <= 0 || cub->map->height <= 0)
		error_exit(cub, "Invalid map size\n", NULL);
	h = cub->map->height;
	w = cub->map->width;
	while (y < cub->map->height)
	{
		x = 0;
		while (x < cub->map->width)
		{
			if (!check_tile(y, x, grid, cub))
				return (0);
			x++;
		}
		y++;
	}
	return (1);
}
