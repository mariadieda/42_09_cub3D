#include"../inc/cub3d.h"

void player_move(t_cub *cub)
{
    int touch_boundaries = 0;

    // todo does new pos touch boundaties? if so, block, i.e. do nothing!
    if (touch_boundaries == 1)
        return;
    cub->velocity = PLAYER_SPEED;
    if (cub->move.backward && check_px_bounds(cub, cub->player_pos.x, cub->player_pos.y + cub->velocity)) //todo also consider player width?
        cub->player_pos.y += cub->velocity;
    if (cub->move.forward && check_px_bounds(cub, cub->player_pos.x, cub->player_pos.y - cub->velocity))
        cub->player_pos.y -= cub->velocity;
    if (cub->move.left && check_px_bounds(cub, cub->player_pos.x - cub->velocity, cub->player_pos.y))
        cub->player_pos.x -= cub->velocity;
    if (cub->move.right && check_px_bounds(cub, cub->player_pos.x + cub->velocity, cub->player_pos.y) )
        cub->player_pos.x += cub->velocity;
    if (cub->move.rotate_left || cub->move.rotate_right)
        return; //todo viewer perspective, not actual psotion change

}