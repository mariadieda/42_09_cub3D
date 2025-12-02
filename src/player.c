#include"../inc/cub3d.h"

float set_rot_angle(float current_angle, int decrease)
{
    float rotate_speed;
    float angle;
    angle = current_angle;
    rotate_speed = PLAYER_ROTATE_SPEED;
    if (decrease)
        rotate_speed *= -1;
    angle += rotate_speed;
    if (angle > 2 * PI)
        angle = 0;
    if (angle < 0)
        angle = 2 * PI;
    return angle;
}

void set_new_pos(t_cub *cub, int decrease_x, int decrease_y)
{
    float x_move;
    float y_move;

    x_move = cos(cub->rot_angle) * PLAYER_SPEED;
    y_move = sin(cub->rot_angle) * PLAYER_SPEED;
    if (decrease_x)
        x_move *= -1;
    if (decrease_y)
        y_move *= -1;

    if (check_px_bounds(cub, cub->player_pos.x + x_move, cub->player_pos.y + y_move))
    {
        //todo also consider player width?
        cub->player_pos.x += x_move;
        cub->player_pos.y += y_move;
    }
}

void player_move(t_cub *cub)
{
    int touch_boundaries = 0;


    // todo does new pos touch boundaties? if so, block, i.e. do nothing!
    if (touch_boundaries == 1)
        return;

    //todo viewer perspective, not actual psotion change
    //should rotation be alwazs possible or should the bounds be checked to, because the player is "square"? --> no, shoudl always be possible
    if (cub->move.rotate_left)
        cub->rot_angle = set_rot_angle(cub->rot_angle, 1);
    if (cub->move.rotate_right)
        cub->rot_angle = set_rot_angle(cub->rot_angle, 0);

    if (cub->move.forward)
        set_new_pos(cub, 0, 0);
    if (cub->move.backward)
        set_new_pos(cub, 1, 1);
    if (cub->move.left)
        set_new_pos(cub, 0, 1);
    if (cub->move.right)
        set_new_pos(cub, 1, 0);

}