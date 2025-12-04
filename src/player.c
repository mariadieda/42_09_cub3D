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



//todo also consider player width?
void update_pos(t_cub *cub, float new_x, float new_y)
{
    printf("\n\ngrid value: %c\n\n", cub->map->grid[(int)new_y][(int)new_x]);
    printf("new player pos:%f, %f\n", new_x, new_y);
    printf("max width, height:%d, %d\n", cub->mlx_data.win_width, cub->mlx_data.win_height);
    printf("map width, height:%d, %d\n", cub->map->width, cub->map->height);
    if (check_walkable_pos(cub, new_x, new_y))
    {
        cub->player_pos.x = new_x;
        cub->player_pos.y = new_y;
    }
    else
        printf("invalid move:%f, %f max width, height:%d, %d\n", cub->player_pos.x, cub->player_pos.y, cub->mlx_data.win_width, cub->mlx_data.win_height);
}


void player_move(t_cub *cub)
{
    //rotation = viewer perspective, not actual position change
    //should rotation be alwazs possible or should the bounds be checked to, because the player is "square"? --> no, shoudl always be possible
    if (cub->move.rotate_left)
        cub->rot_angle = set_rot_angle(cub->rot_angle, 1);
    if (cub->move.rotate_right)
        cub->rot_angle = set_rot_angle(cub->rot_angle, 0);

    float cos_angle;
    float sin_angle;
    float new_x = 0;
    float new_y = 0;

    cos_angle = cos(cub->rot_angle) * PLAYER_SPEED;
    sin_angle = sin(cub->rot_angle) * PLAYER_SPEED;

    if (cub->move.forward)
    {
        new_x = cub->player_pos.x + cos_angle;
        new_y = cub->player_pos.y - sin_angle;
        update_pos(cub, new_x, new_y);
    }
    if (cub->move.backward)
    {
        new_x = cub->player_pos.x - cos_angle;
        new_y = cub->player_pos.y + sin_angle;
        update_pos(cub, new_x, new_y);
    }
    if (cub->move.left)
    {
        new_x = cub->player_pos.x - sin_angle;
        new_y = cub->player_pos.y + cos_angle;
        update_pos(cub, new_x, new_y);
    }
    if (cub->move.right){
        new_x = cub->player_pos.x + sin_angle;
        new_y = cub->player_pos.y - cos_angle;
        update_pos(cub, new_x, new_y);
    }
}