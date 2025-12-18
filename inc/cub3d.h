/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 11:16:23 by mdiederi          #+#    #+#             */
/*   Updated: 2025/12/16 11:16:28 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef INC_42_09_CUBE3D_CUBE3D_H
# define INC_42_09_CUBE3D_CUBE3D_H

# define PLAYER_SPEED 5           
	//how fast the player is moving (how many tile units per keypress)
# define PLAYER_ROTATE_SPEED 0.02f //how fast the player is turning

//# define PI 3.14159265359
# define PI 3.1415926535897932384626
	//whats highest precision possible/necessary?

# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363
# define DEBUG 0

# include "../gnl/get_next_line.h"
# include "../libft/ft_printf.h"
# include "../libft/libft.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <fcntl.h> // open
# include <math.h>
# include <mlx.h>
# include <stdio.h>
//# include "minilibx-linux/mlx.h"
# include <stdlib.h> // malloc, free, exit
# include <unistd.h> // close, read, write

typedef struct s_mlx_data
{
	int				bits_per_pixel;
	int				line_length;
	int				endian;
	int				screen_width;
	int				screen_height;
	int				win_width;
	int				win_height;
}					t_mlx_data;

typedef struct s_tex {
	void *img;
	char *pxl_arr;
	int width;
	int height;
	int bpp;
	int line_len;
	int endian;
} t_tex;

typedef struct s_col
{
	char			*no_tex_p;
	char			*ea_tex_p;
	char			*so_tex_p;
	char			*we_tex_p;
	struct s_tex	wall_tex[4];
	int				floor;
	int				ceil;
	int				has_floor;
	int				has_ceil;

}					t_col;

typedef struct s_map
{
	char			**grid;
	int				width;
	int				height;
	int				capacity;
}					t_map;

typedef struct s_pos
{
	float			x;
	float			y;
}					t_pos;

typedef struct s_int_pos
{
	int				x;
	int				y;
}					t_int_pos;

typedef struct s_dir
{
	double			dir_x;
	double			dir_y;
	double			plane_x;
	double			plane_y;
}					t_dir;

typedef struct s_move
{
	int				forward;
	int				backward;
	int				left;
	int				right;
	int				rotate_left;
	int				rotate_right;
}					t_move;

typedef struct s_line
{
	struct s_pos	start_point;
	struct s_pos	end_point;
	double			line_len;
	double			x_diff;
	double			y_diff;
}					t_line;

typedef struct s_cub
{
	struct s_map	*map;
	t_mlx_data		mlx_data;
	void			*mlx;
	void			*win;
	void			*img;
	char			*pxl_arr;
	struct s_col	*col;
	//t_player		*player;
	char			spawn_dir;
	struct s_pos player_tile; //only used for parsing
	struct s_pos	player_px;
	float			player_angle;
	float			player_fov;
	float			fraction_ray_angle;
	float			screen_dist;
	struct s_move	move;
	int				tile_size;

}					t_cub;

//main
void				make_window(t_cub *cub);

//parse
int					parse_file(char *filename, t_cub *cub);

//keyhandlers
int					handle_keyrelease(int keycode, t_cub *cub);
int					handle_keypress(int keycode, t_cub *cub);
int					handle_close(t_cub *cub);

//rendering
int					render(t_cub *cub);


// collision handling
int					check_screen_bounds_px(t_cub *cub, float x_px, float y_px);
int					check_map_bounds_tiles(t_cub *cub, int x_tile, int y_tile);
int					check_walkable_pos(t_cub *cub, float new_x_px,
						float new_y_px);

//2D debug
void				draw_tile(t_cub *cub, t_pos start_px,
				              float size_factor, int color);
void				draw_map(t_cub *cub, int color);
void				draw_player_in_minimap(t_cub *cub, int wall_color,
						int triangle_color);
void				draw_player_triangle(t_cub *cub, float angle, float size,
						int color);

//player
void				player_move(t_cub *cub);
void				draw_ray(t_cub *cub, float start_angle, int i);
void				draw_line(t_cub *cub, t_pos start_pos_px, t_pos end_pos_px,
						int color);
void				set_last_ray_point(t_cub *cub, float start_angle,
						t_pos *ray_px);
void				try_put_pixel(t_cub *cub, float x_px, float y_px,
						int color);
void				draw_vertical_slices(t_cub *cub, int i, t_pos *ray_px,
				                         float ray_angle);
float				set_player_spawn_dir(char c);
int					touches_wall(t_cub *cub, int x_tile, int y_tile);
int					check_map_bounds_tiles(t_cub *cub, int x_tile, int y_tile);

// texture handling
void				create_texture_imgs(t_cub *cub);
int					get_texture_px_color(t_cub *cub, float wall_height, t_pos pos);

//clean up
void				clean_up(t_cub *cub);
void				error_exit(t_cub *cub, char *msg, char **to_be_freed);
void				free_n_array(char **arr, int grid_height);

//utils
void				print_map(t_cub *cub);
int					ft_fmin(double x, double y);
t_int_pos			get_map_tile_int_pos(t_cub *cub, t_pos px_pos);
t_pos				get_map_tile_px_pos(t_cub *cub, t_pos px_pos);
#endif //INC_42_09_CUBE3D_CUBE3D_H
