/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 16:09:28 by mdiederi          #+#    #+#             */
/*   Updated: 2025/10/16 16:09:33 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INC_42_09_CUBE3D_CUBE3D_H
#define INC_42_09_CUBE3D_CUBE3D_H

# include "gnl/get_next_line.h"
# include "libft/ft_printf.h"
# include "libft/libft.h"
# include <fcntl.h> // open
# include <math.h>
# include <mlx.h>
# include <stdlib.h> // malloc, free, exit
# include <unistd.h> // close, read, write

typedef struct s_mlx_data
{
    int				bits_per_pixel;
    int				line_length;
    int				endian;
    int				map_max_width;
    int				map_max_height;
    int				screen_width;
    int				screen_height;
    int				win_width;
    int				win_height;
    unsigned int	def_color;
    double			zoom_factor;
    int				offset_x;
    int				offset_y;
}					t_mlx_data;

typedef struct s_cub
{
    t_map			*map;
    t_mlx_data		mlx_data;
    void			*mlx;
    void			*win;
    void			*img;
    char			*pxl_arr;

}					t_cub;

#endif //INC_42_09_CUBE3D_CUBE3D_H
