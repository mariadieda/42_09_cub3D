/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 10:13:33 by mdiederi          #+#    #+#             */
/*   Updated: 2025/10/20 10:14:15 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void init_cub_for_rendering(t_cub *cub)
{
	cub->mlx_data.bits_per_pixel = 24;
	cub->mlx_data.endian = 0;

	cub->mlx = mlx_init();
	if (!cub->mlx)
		error_exit(cub, "Failed to initialize MLX\n", NULL);

	printf("map w=%d h=%d\n", cub->map->width, cub->map->height);

	mlx_get_screen_size(
		cub->mlx,
		&cub->mlx_data.screen_width,
		&cub->mlx_data.screen_height
	);

	cub->mlx_data.tile_size = ft_fmin(
		cub->mlx_data.screen_width  / cub->map->width,
		cub->mlx_data.screen_height / cub->map->height
	);

	if (cub->mlx_data.tile_size < 1)
		error_exit(cub, "Map too large for screen\n", NULL);

	cub->mlx_data.win_width  = cub->mlx_data.tile_size * cub->map->width;
	cub->mlx_data.win_height = cub->mlx_data.tile_size * cub->map->height;

	printf("window w=%d h=%d, map w*CUBE %d, map h*CUBE %d\n",
		cub->mlx_data.win_width, cub->mlx_data.win_height,
		cub->map->width * cub->mlx_data.tile_size,
		cub->map->height * cub->mlx_data.tile_size);

	cub->rot_angle = PI / 2;
}



int	has_cub_extension(char *cub_fn)
{
	char * extension;

	if (!cub_fn)
		return (0);
	extension = ft_strrchr(cub_fn, '.');	
	if(!extension || ft_strncmp(extension, ".cub", 4) != 0)
		return (0);
	return (1);
}

void normalize_map(t_cub *cub)
{
	int i;
	int j;

	for (i = 0; i < cub->map->height; i++)
	{
		char *old = cub->map->grid[i];
		char *new = ft_calloc(cub->map->width + 1, 1);

		j = 0;
		while (old[j])
		{
			new[j] = old[j];
			j++;
		}
		while (j < cub->map->width)
			new[j++] = ' ';

		free(old);
		cub->map->grid[i] = new;
	}
}


int	main(int argc, char **argv)
{
	t_cub	cub;

	ft_memset(&cub, 0, sizeof(t_cub));
	if (argc != 2 || !has_cub_extension(argv[1]))
		error_exit(&cub,"Please provide exactly one valid .cub file as an argument\n", NULL);
	parse_file(argv[1], &cub);
	printf("map w=%d h=%d\n", cub.map->width, cub.map->height);
	if (cub.map->width <= 0 || cub.map->height <= 0)
		error_exit(&cub,"Invalid map size\n", NULL);
	//normalize_map(&cub);
	// todo rotate_map(&cub); ??
	print_map(&cub); //todo rm debug

	init_cub_for_rendering(&cub);
	make_window(&cub);
	mlx_hook(cub.win, 2, 1L << 0, handle_keypress, &cub);
	mlx_hook(cub.win, 3, 1L << 1, handle_keyrelease, &cub);
	mlx_hook(cub.win, 17, 0, handle_close, &cub);
	mlx_loop_hook(cub.mlx, render, &cub);
	mlx_loop(cub.mlx);
	//clean_up(&cub); //after mlx_loop nothing runs
	return (0);
}
