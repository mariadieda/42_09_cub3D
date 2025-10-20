/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 16:09:28 by mdiederi          #+#    #+#             */
/*   Updated: 2025/10/16 16:09:33 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INC_42_09_CUBE3D_CUBE3D_H
#define INC_42_09_CUBE3D_CUBE3D_H

# define WIDTH 1280 //todo or define dynamically with size fun?
# define HEIGHT 720

# define CUBE_SIZE = 64
# define PLAYER_HEIGHT (CUBE_SIZE / 2)

//# define PI 3.14159265359
# define PI 3.1415926535897932384626 //whats highest precision possible/necessary?

# define W 119
# define A 97
# define S 115
# define D 100

# define LEFT 65361
# define RIGHT 65363

#include "./mlx/mlx.h"
#include <stdio.h>
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

typedef struct s_rgb
{
    int					r;
    int					g;
    int					b;
}						t_rgb;

typedef struct s_col
{
    char			*no_tex_p;
    char		    *so_tex_p;
    char			*we_tex_p;
    char			*ea_tex_p;
    struct s_rgb	floor;
    struct s_rgb	ceil;
}					t_col;

typedef struct s_pos{
    int x;
    int y;
}t_pos;

typedef struct s_cub
{
    t_map			*map;
    t_mlx_data		mlx_data;
    void			*mlx;
    void			*win;
    void			*img;
    char			*pxl_arr;
    struct s_col    *col;
    char            spawn_dir;
    struct s_pos    player_pos;
}					t_cub;

#endif //INC_42_09_CUBE3D_CUBE3D_H
