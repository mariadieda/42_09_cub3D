
#ifndef INC_42_09_CUBE3D_CUBE3D_H
#define INC_42_09_CUBE3D_CUBE3D_H

# define WIDTH 1280 //todo or define dynamically with size fun?
# define HEIGHT 720

# define CUBE_SIZE 64
# define PLAYER_HEIGHT (CUBE_SIZE / 2)

//# define PI 3.14159265359
# define PI 3.1415926535897932384626 //whats highest precision possible/necessary?

# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363

# include <X11/X.h>
# include <X11/keysym.h>
#include <stdio.h>
# include "../gnl/get_next_line.h"
# include "../libft/ft_printf.h"
# include "../libft/libft.h"
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
    int				screen_width;
    int				screen_height;
    int				win_width;
    int				win_height;
}					t_mlx_data;

typedef struct s_rgb
{
    int				r;
    int				g;
    int				b;
}					t_rgb;

typedef struct s_col
{
    char			*no_tex_p;
    char		    *so_tex_p;
    char			*we_tex_p;
    char			*ea_tex_p;
    int             floor_color;
    int             ceil_color;
    struct s_rgb	floor;
    struct s_rgb	ceil;
    int has_floor;
    int has_ceil;
      
}					t_col;

typedef struct s_map {
    char  **grid;
    int  width;
    int  height;
    int  capacity;
}                   t_map;

typedef struct s_pos {
    double x;
    double y;
}                   t_pos;

typedef struct s_dir {
    double dir_x;
    double dir_y;
    double plane_x;
    double plane_y;
}                   t_dir;

typedef struct s_move
{
    int forward;
    int backward;
    int left;
    int right;
    int rotate_left;
    int rotate_right;
}                   t_move;

typedef struct s_cub
{
    struct s_map    *map;
    t_mlx_data		mlx_data;
    void			*mlx;
    void			*win;
    void			*img;
    char			*pxl_arr;
    struct s_col    *col;
    char            spawn_dir;
    struct s_pos    player_pos;
    struct s_move   move;
}					t_cub;
//we can add int player_tile_x and int player_tile_y if it helps you (i.e:to check collisions). If not you just use: (int)player_pos.x

void	make_window(t_cub *cub);
void    clean_up(t_cub *cub);
void	error_exit(t_cub *cub, char *msg, char** to_be_freed);
int     parse_file(char* filename, t_cub* cub);
void    free_array(char **arr, int grid_height);

#endif //INC_42_09_CUBE3D_CUBE3D_H
