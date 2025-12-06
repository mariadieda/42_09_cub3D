#include "../inc/cub3d.h"

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
    //printf("keycode pressed: %d\n", keycode);
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
    //printf("keyrelease pressed: %d\n", keycode);
    return (0);
}

