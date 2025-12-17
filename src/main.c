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

/*
 * Set graphics params (e.g. tile size, window dims) and player const settings,
 * such as player angle, FOV (to 60 deg) +
 * convert tile player pos to px pos
 */
void	init_cub_for_rendering(t_cub *cub)
{
	cub->mlx = mlx_init();
	if (!cub->mlx)
		error_exit(cub, "Failed to initialize MLX\n", NULL);
	mlx_get_screen_size(cub->mlx, &cub->mlx_data.screen_width,
			&cub->mlx_data.screen_height);
	cub->tile_size = ft_fmin(
		(double)cub->mlx_data.screen_width / cub->map->width,
		(double)cub->mlx_data.screen_height / cub->map->height);
	if (cub->tile_size < 1)
		error_exit(cub, "Error\nMap too large for screen\n", NULL);
	cub->mlx_data.win_width = cub->tile_size * cub->map->width;
	cub->mlx_data.win_height = cub->tile_size * cub->map->height;
	cub->player_px.x = (float)cub->tile_size * cub->player_tile.x;
	cub->player_px.y = (float)cub->tile_size * cub->player_tile.y;
	printf("spanw dir: %c\n", cub->spawn_dir);
	cub->player_angle = set_player_spawn_dir(cub->spawn_dir);
	cub->player_fov = PI / 3;
	cub->fraction_ray_angle = cub->player_fov / (float)cub->mlx_data.win_width;
	cub->screen_dist = ((float)cub->mlx_data.win_width / 2.00f)
		/ tanf(cub->player_fov / 2.00f);
}
/*
* 	printf("map w=%d h=%d\nwindow w=%d h=%d, map w*CUBE %d, map h*CUBE %d\n",
* 			cub->map->width, cub->map->height
			cub->mlx_data.win_width,
			cub->mlx_data.win_height,
			cub->map->width * cub->tile_size,
			cub->map->height * cub->tile_size);
			//todo rm debug
 */

int	has_cub_extension(char *cub_fn)
{
	char	*extension;

	if (!cub_fn)
		return (0);
	extension = ft_strrchr(cub_fn, '.');
	if (!extension || ft_strncmp(extension, ".cub", 4) != 0)
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	t_cub	cub;

	ft_memset(&cub, 0, sizeof(t_cub));
	if (argc != 2 || !has_cub_extension(argv[1]))
	{
		error_exit(&cub, "Please provide exactly one "
			"valid .cub file as an argument\n", NULL);
	}
	parse_file(argv[1], &cub);
	print_map(&cub); //todo rm debug
	init_cub_for_rendering(&cub);
	make_window(&cub);
	mlx_hook(cub.win, 2, 1L << 0, handle_keypress, &cub);
	mlx_hook(cub.win, 3, 1L << 1, handle_keyrelease, &cub);
	mlx_hook(cub.win, 17, 0, handle_close, &cub);
	mlx_loop_hook(cub.mlx, render, &cub);
	mlx_loop(cub.mlx);
	return (0);
}
