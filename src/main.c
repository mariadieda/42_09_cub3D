/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 16:08:43 by mdiederi          #+#    #+#             */
/*   Updated: 2025/10/16 16:08:48 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	init_map(t_cub *cub)
{
    t_map	*map;

    map = malloc(sizeof(t_map));
    map->width = 0;
    map->height = 0;
    map->grid = NULL;
    map->min_z = 2147483647;
    map->max_z = -2147483647;
    map->min_x_pr = 2147483647;
    map->max_x_pr = -2147483647;
    map->min_y_pr = 2147483647;
    map->max_y_pr = -2147483647;
    cub->map = map;
}

void	init_cub(t_cub *cub)
{
    //init_map(cub);
    cub->mlx = NULL;
    cub->win = NULL;
    cub->img = NULL;
    cub->pxl_arr = NULL;
    cub->mlx_data.bits_per_pixel = 24;
    cub->mlx_data.endian = 0;
    cub->mlx_data.map_max_width = 0;
    cub->mlx_data.map_max_height = 0;
    cub->mlx_data.screen_width = 0;
    cub->mlx_data.screen_height = 0;
    cub->mlx_data.win_width = 0;
    cub->mlx_data.win_height = 0;
    cub->mlx_data.zoom_factor = 1;
}

int	handle_close(t_cub *cub)
{
    cleanup_cub(cub);
    exit(0);
    return (0);
}

int	handle_keypress(int keycode, t_cub *cub)
{
    if (keycode == 65307)
        handle_close(cub);
    return (0);
}

int	main(int argc, char **argv)
{
    t_cub    cub;

    ft_memset(&cub, 0, sizeof(t_cub));
    if (argc != 2 ) // || !check_file_ext(argc, argv)
        error_exit(&cub,
            "Please provide exactly one valid .cub file as an argument\n");
    init_cub(&cub);
    // todo read_in_lines(&cub, argv[1]);
    // todo normalize_rotate_map(&cub);
    make_window(&cub);
    mlx_key_hook(cub.win, handle_keypress, &cub);
    mlx_hook(cub.win, 17, 0, handle_close, &cub);
    mlx_loop(cub.mlx);
    cleanup_cub(&cub);
    return (0);
}
