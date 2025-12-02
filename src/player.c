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
/*
float set_new_pos(t_cub *cub, int decrease, char axis)
{
    float cos_rota;
    float sin_rota;


    cos_rota = cos(cub->rot_angle);
    sin_rota = sin(cub->rot_angle);
    int rotate_speed;
    float angle;
    angle = cub->rot_angle;
    rotate_speed = PLAYER_ROTATE_SPEED;
    if (decrease)
        rotate_speed *= -1;
    angle += rotate_speed;
    if (angle > 2 * PI)
        angle = 0;
    if (angle < 0)
        angle = 2 * PI;
    return angle;
}*/

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
        cub->rot_angle = set_rot_angle(cub->rot_angle, 1);

    cub->velocity = PLAYER_SPEED;
    if (cub->move.backward && check_px_bounds(cub, cub->player_pos.x, cub->player_pos.y + PLAYER_SPEED)) //todo also consider player width?
        cub->player_pos.y += PLAYER_SPEED;
    if (cub->move.forward && check_px_bounds(cub, cub->player_pos.x, cub->player_pos.y - PLAYER_SPEED))
        cub->player_pos.y -= PLAYER_SPEED;
    if (cub->move.left && check_px_bounds(cub, cub->player_pos.x - PLAYER_SPEED, cub->player_pos.y))
        cub->player_pos.x -= PLAYER_SPEED;
    if (cub->move.right && check_px_bounds(cub, cub->player_pos.x + PLAYER_SPEED, cub->player_pos.y) )
        cub->player_pos.x += PLAYER_SPEED;

}