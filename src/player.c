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

int check_wall(char **grid, float new_x, float new_y)
{
    if (grid[(int)new_y][(int)new_x])
        return 1;
    return 0;

}

//todo also consider player width?
void set_new_pos(t_cub *cub, int decrease_cos, int decrease_sin, int strife)
{
    float cos_angle;
    float sin_angle;
    float new_x;
    float new_y;

    cos_angle = cos(cub->rot_angle) * PLAYER_SPEED;
    sin_angle = sin(cub->rot_angle) * PLAYER_SPEED;
    if (decrease_cos)
        cos_angle *= -1;
    if (decrease_sin)
        sin_angle *= -1;
    if (strife)
    {
        new_x = sin_angle + cub->player_pos.x;
        new_y = cos_angle + cub->player_pos.x;
    }
    else
    {
        new_x = cos_angle + cub->player_pos.x;
        new_y = sin_angle + cub->player_pos.x;
    }
    int i = 0;
    while(cub->map->grid[i])
    {
        printf("\n%s\n\n",cub->map->grid[i]);
        i++;
    }
    printf("\n\n\n\n\nn\n%d\n\n\n\\n\nn\n\\n\nn\n", cub->map->grid[(int)new_y][(int)new_x]);
    // todo does new pos touch boundaties? if so, block, i.e. do nothing!
    if (check_px_bounds(cub, (int)new_x, (int)new_y) && !cub->map->grid[(int)new_y][(int)new_x])
    {
        printf("\n\n\n\n\nn\n\n\n\n\\n\nn\n\\n\nn\n");
        cub->player_pos.x = new_x;
        cub->player_pos.y = new_y;
    }
}

void update_pos(t_cub *cub,


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
        new_y = cub->player_pos.y + sin_angle;
    }
    if (cub->move.backward)
    {
        new_x = cub->player_pos.x - cos_angle;
        new_y = cub->player_pos.y - sin_angle;
    }
    if (cub->move.left)
    {
        new_x = cub->player_pos.x + sin_angle;
        new_y = cub->player_pos.y - cos_angle;
    }
    if (cub->move.right){
        new_x = cub->player_pos.x - sin_angle;
        new_y = cub->player_pos.y + cos_angle;
        update_pos()
    }
    if (check_px_bounds(cub, (int)new_x, (int)new_y) && !cub->map->grid[(int)new_y][(int)new_x])
    {
        printf("\n\n\n\n\nn\n\n\n\n\\n\nn\n\\n\nn\n");
        cub->player_pos.x = new_x;
        cub->player_pos.y = new_y;
    }

}