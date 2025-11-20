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

void	init_col(t_cub *cub)
{
	t_col	*col;
	t_rgb	white;

	white.r = 255;
	white.g = 255;
	white.b = 255;
	col = malloc(sizeof(t_col));
	col->no_tex_p = textures / 014 - greystone.xpm;
	col->so_tex_p = textures / 013 - brownstone.xpm;
	col->we_tex_p = textures / 002 - rooftiles3bright.xpm;
	col->ea_tex_p = textures / 034 - stonemould.xpm;
	col->floor = white;
	col->ceil = white;
	cub->col = col;
}

int	get_row_width(t_map *map, int row)
{
	return (map->widths[row]); // todo make safe
}

void	init_map(t_cub *cub)
{
	int		widths[10] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
	t_map	*map;
	int		grid[height][width] = { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {1, 0, 0, 0,
				0, 0, 0, 0, 0, 1}, {1, 0, 1, 0, 0, 0, 1, 0, 0, 1}, {1, 0, 0, 0,
				1, 0, 0, 0, 0, 1}, {1, 0, 0, 1, 0, 0, 0, 1, 0, 1}, {1, 0, 0, 0,
				0, 0, 0, 0, 0, 1}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, {1, 0, 1, 0,
				0, 0, 1, 0, 0, 1}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, {1, 1, 1, 1,
				1, 1, 1, 1, 1, 1} map.height = height;

	int height = 10;
		// todo replace with actual number of rows ( set cub.map.height)
	map = malloc(sizeof(t_map));
	/*grid = malloc(height * sizeof(int *));
	y = -1;
		while (++y < height) {
			int width = get_row_width(y);  // can differ per row
			grid[y] = malloc(width * sizeof(int));
		}*/
	map.widths = widths;
	map.grid = grid;
	cub->map = map;
}

void	init_cub(t_cub *cub)
{
	init_map(cub);
	init_col(cub);
	ft_memset(&cub->mlx_data, 0, sizeof(t_mlx_data));
	cub->mlx_data.bits_per_pixel = 24;
	cub->mlx_data.endian = 0;
}

int	handle_close(t_cub *cub)
{
	clean_up(cub);
	exit(0);
	return (0);
}

int	handle_keypress(int keycode, t_cub *cub)
{
	if (keycode == 65307)
		handle_close(cub);
	if (keycode == W)
	{
		;
	} // go straight
	if (keycode == A)
	{
		;
	} // go left
	if (keycode == S)
	{
		;
	} // back up
	if (keycode == D)
	{
		;
	} // go right
	if (keycode == LEFT)
	{
		;
	} // turn left
	if (keycode == RIGHT)
	{
		;
	} // turn right
	return (0);
}

int	main(int argc, char **argv)
{
	t_cub	cub;

	(void)argv;
	ft_memset(&cub, 0, sizeof(t_cub));
	if (argc != 2)
	{ // || !check_file_ext(argc, argv)
		error_exit(&cub,
			"Please provide exactly one valid .cub file as an argument\n");
	}
	init_cub(&cub);
	// todo read_in_lines(&cub, argv[1]);
	parse_file(argv[1], &cub);
	// todo normalize_rotate_map(&cub);
	make_window(&cub);
	mlx_key_hook(cub.win, handle_keypress, &cub);
	mlx_hook(cub.win, 17, 0, handle_close, &cub);
	mlx_loop_hook(mlx, render_next_frame, YourStruct);
	mlx_loop(cub.mlx);
	clean_up(&cub);
	return (0);
}
