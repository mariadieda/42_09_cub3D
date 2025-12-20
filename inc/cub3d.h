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

#ifndef CUB3D_H
# define CUB3D_H

# define PLAYER_SPEED 5           //how fast the player is moving (how many tile units per keypress)
# define PLAYER_ROTATE_SPEED 0.03f //how fast the player is turning

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
	int				bytes_per_pixel;
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
	int bits_per_pixel;
	int bytes_per_pixel;
	int line_len;
	int endian;
} t_tex;

struct			s_w_slice
{
	float		wall_height;
	int			wall_start_y;
	int			wall_end_y;
	int			clipped;
	float		corrected_dist;
	float		tex_step;
	float		tex_pos;
	char		*tex_px_wo_y;
} typedef t_w_slice;


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


typedef struct s_hit
{
	struct s_int_pos step;
	struct s_int_pos map_pos;
	struct s_pos player_float_map_pos;
	struct s_pos ray_dir;
	struct s_pos delta_dist;
	struct s_pos side_dist;
	struct s_tex tex;
	double wall_x;
	int rel_pos_x;
	int is_horiz ;  // 0 = vertical, 1 = horizontal
	float perp_dist; // distance from player to wall
	struct s_pos hit_point;     // exact hit position in tile (for texture)
} t_hit;

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

struct s_minimap
{
	t_int_pos offset;
	float scale;
	int   tile_px;
	int player_color;
	int wall_color;
	int space_color;
	int walk_color;
} typedef t_minimap;

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

void				draw_minimap(t_cub *cub);
void	draw_tile(t_cub *cub, t_minimap mm, t_pos map_px, int color);
void				draw_map(t_cub *cub, t_minimap mm);
void				draw_player_in_minimap(t_cub *cub, int wall_color,
						int triangle_color);
void				draw_player_triangle(t_cub *cub, t_pos player_pos, float angle, float scale, t_minimap mm);

//player
void				player_move(t_cub *cub);
void				draw_ray(t_cub *cub, float start_angle, int i);
void				draw_line(t_cub *cub, t_pos start_pos_px, t_pos end_pos_px,
						int color);
void				set_last_ray_point(t_cub *cub, float start_angle,
						t_pos *ray_px);
void				try_put_pixel(t_cub *cub, float x_px, float y_px,
						int color);
void				draw_vertical_slices(t_cub *cub, int i, t_hit *hit, float start_angle);
float				set_player_spawn_dir(char c);
int					touches_wall(t_cub *cub, int x_tile, int y_tile);
int					check_map_bounds_tiles(t_cub *cub, int x_tile, int y_tile);

// texture handling
t_tex				select_texture(t_cub *cub, t_pos ray_dir, int is_horiz_hit);
void				create_texture_imgs(t_cub *cub);
//int					get_texture_px_color(t_cub *cub, float wall_height, t_pos pos);
int					get_texture_px_color(t_hit *hit, float wall_height, int wall_start, int y, int clipped);

//clean up
void				clean_up(t_cub *cub);
void				error_exit(t_cub *cub, char *msg, char **to_be_freed);
void				free_n_array(char **arr, int grid_height);

//utils
void				print_map(t_cub *cub);
int					ft_fmin(double x, double y);
t_int_pos			get_map_tile_int_pos(t_cub *cub, t_pos px_pos);
t_pos				get_map_tile_px_pos(t_cub *cub, t_pos px_pos);
#endif //CUB3D_H
