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

/*void	init_col(t_cub *cub)
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
	grid = malloc(height * sizeof(int *));
	y = -1;
		while (++y < height) {
			int width = get_row_width(y);  // can differ per row
			grid[y] = malloc(width * sizeof(int));
		}
	map.widths = widths;
	map.grid = grid;
	cub->map = map;
}*/

void	init_cub(t_cub *cub)
{
	ft_memset(cub, 0, sizeof(t_cub));
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
	if (keycode == XK_Escape)
		handle_close(cub);
	if (keycode == XK_W || keycode == XK_w )
		cub->move.forward = 1;
	if (keycode == XK_A || keycode == XK_a)
		cub->move.left = 1;
	if (keycode == XK_S || keycode == XK_s)
		cub->move.backward = 1;
	if (keycode == XK_D || keycode == XK_d)
		cub->move.right = 1;
	if (keycode == XK_Left)
		cub->move.rotate_left = 1;
	if (keycode == XK_Right)
		cub->move.rotate_right = 1;
	printf("keycode pressed: %d\n", keycode);
	return (0);
}

int	handle_keyrelease(int keycode, t_cub *cub)
{
	if (keycode == XK_W || keycode == XK_w )
		cub->move.forward = 0;
	if (keycode == XK_A || keycode == XK_a)
		cub->move.left = 0;
	if (keycode == XK_S || keycode == XK_s)
		cub->move.backward = 0;
	if (keycode == XK_D || keycode == XK_d)
		cub->move.right = 0;
	if (keycode == XK_Left)
		cub->move.rotate_left = 0;
	if (keycode == XK_Right)
		cub->move.rotate_right = 0;
	return (0);
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

double ft_fmin(double x, double y)
{
	if (x >= y)
		return x;
	return y;
}


int	main(int argc, char **argv)
{
	t_cub	cub;

	(void)argv;
	ft_memset(&cub, 0, sizeof(t_cub));
	if (argc != 2 || !has_cub_extension(argv[1]))
	{
		error_exit(&cub,
			"Please provide exactly one valid .cub file as an argument\n", NULL);
	}
	parse_file(argv[1], &cub);
	int i = 0;
	while(cub.map->grid[i])
	{
		printf("\n%s\n\n",cub.map->grid[i]);
		i++;
	}
	normalize_map(&cub);
	// todo rotate_map(&cub); ??
	make_window(&cub);
	cub.map->tile_size = ft_fmin(cub.mlx_data.win_width  / cub.map->width,cub.mlx_data.win_height / cub.map->height);

	printf("map w=%d h=%d | window w=%d h=%d, map_width  * CUBE_SIZE: %d, map_height  * CUBE_SIZE: %d\n", cub.map->width, cub.map->height,cub.mlx_data.win_width, cub.mlx_data.win_height, cub.map->width*cub.map->tile_size, cub.map->height * cub.map->tile_size);

	cub.rot_angle = PI / 2; //todo set to orientation read from map
	mlx_hook(cub.win, 2, 1L << 0, handle_keypress, &cub);
	mlx_hook(cub.win, 3, 1L << 1, handle_keyrelease, &cub);
	mlx_hook(cub.win, 17, 0, handle_close, &cub);
	mlx_loop_hook(cub.mlx, render, &cub);
	mlx_loop(cub.mlx);
	//clean_up(&cub); //after mlx_loop nothing runs
	return (0);
}
