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
*/
void	init_cub(t_cub *cub)
{
    //init_map(cub);
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
        {;} // go straight
	if (keycode == A)
        {;} // go left
	if (keycode == S)
        {;} // back up
	if (keycode == D)
        {;} // go right
	if (keycode == LEFT)
        {;} // turn left
	if (keycode == RIGHT)
        {;} // turn right
    return (0);
}

int	main(int argc, char **argv)
{
    t_cub    cub;
    (void) argv;

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
    clean_up(&cub);
    return (0);
}
