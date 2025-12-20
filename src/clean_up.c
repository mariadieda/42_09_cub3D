/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:15:39 by mdiederi          #+#    #+#             */
/*   Updated: 2025/12/16 10:15:47 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	free_n_array(char **arr, int grid_height)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (i < grid_height)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_textures(t_cub *cub)
{
	int	i;

	if (cub->col->no_tex_p)
		free(cub->col->no_tex_p);
	if (cub->col->so_tex_p)
		free(cub->col->so_tex_p);
	if (cub->col->we_tex_p)
		free(cub->col->we_tex_p);
	if (cub->col->ea_tex_p)
		free(cub->col->ea_tex_p);
	if (cub->mlx && cub->col->wall_tex->img)
	{
		i = 0;
		while (i < 4)
		{
			mlx_destroy_image(cub->mlx, cub->col->wall_tex[i].img);
			i++;
		}
	}
	free(cub->col);
}

void	clean_up(t_cub *cub)
{
	if (!cub)
		return ;
	if (cub->col)
		free_textures(cub);
	if (cub->mlx)
	{
		if (cub->img)
			mlx_destroy_image(cub->mlx, cub->img);
		if (cub->win)
			mlx_destroy_window(cub->mlx, cub->win);
		mlx_destroy_display(cub->mlx);
		free(cub->mlx);
	}
	if (cub->map)
	{
		free_n_array(cub->map->grid, cub->map->height);
		free(cub->map);
	}
}

/* only for usage when calling with array on stack: error_exit(cub,
"Error\nMalloc failed!\n", (char*[]){trimd, NULL});*/
void	free_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
}

void	error_exit(t_cub *cub, char *msg, char **to_be_freed)
{
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(msg, 2);
	free_array(to_be_freed);
	clean_up(cub);
	exit(1);
}
