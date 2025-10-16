/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 16:08:57 by mdiederi          #+#    #+#             */
/*   Updated: 2025/10/16 16:09:01 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	make_window(t_fdf *fdf)
{
    double	used_width;
    double	used_height;

    fdf->mlx = mlx_init();
    if (!fdf->mlx)
        error_exit(fdf, "Failed to initialize MLX\n");
    mlx_get_screen_size(fdf->mlx, &fdf->mlx_data.screen_width,
        &fdf->mlx_data.screen_height);
    fdf->mlx_data.win_width = (int)round(fdf->mlx_data.screen_width * 0.8);
    fdf->mlx_data.win_height = (int)round(fdf->mlx_data.screen_height * 0.8);
    set_max_map_values(fdf);
    calculate_zoom_factor(fdf, 0.9, fdf->mlx_data.win_width,
        fdf->mlx_data.win_height);
    used_width = fdf->mlx_data.map_max_width * fdf->mlx_data.zoom_factor;
    used_height = fdf->mlx_data.map_max_height * fdf->mlx_data.zoom_factor;
    fdf->mlx_data.offset_x = (fdf->mlx_data.win_width - used_width) / 2.0;
    fdf->mlx_data.offset_y = (fdf->mlx_data.win_height - used_height) / 2.0;
    fdf->win = mlx_new_window(fdf->mlx, fdf->mlx_data.win_width,
            fdf->mlx_data.win_height, "fil d'enfer");
    if (!fdf->win)
        error_exit(fdf, "Failed to create window\n");
    make_image(fdf);
    mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->img, 0, 0);
}
