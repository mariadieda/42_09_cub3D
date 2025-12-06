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
/*void update_pos(t_cub *cub, float new_x, float new_y)
{
    printf("\n\ngrid value: %c\n\n", cub->map->grid[(int)new_y][(int)new_x]);
    printf("new player pos:%f, %f\n", new_x, new_y);
    printf("max width, height:%d, %d\n", cub->mlx_data.win_width, cub->mlx_data.win_height);
    printf("map width, height:%d, %d\n", cub->map->width, cub->map->height);
    *//*if (check_walkable_pos(cub, new_x, new_y))
    {
        cub->player_pos.x = new_x;
        cub->player_pos.y = new_y;
    }*//*
    if (check_walkable_pos(cub, cub->player_pos.x - new_x, cub->player_pos.y))
        cub->player_pos.x = new_x;
    else
        printf("invalid move x coord:%f, %f max width, height:%d, %d\n", cub->player_pos.x, cub->player_pos.y, cub->mlx_data.win_width, cub->mlx_data.win_height);
    if (check_walkable_pos(cub, cub->player_pos.x, cub->player_pos.y - new_y))
        cub->player_pos.y = new_y;
    else
        printf("invalid move y coord:%f, %f max width, height:%d, %d\n", cub->player_pos.x, cub->player_pos.y, cub->mlx_data.win_width, cub->mlx_data.win_height);
}*/

void update_pos(t_cub *cub, float new_x, float new_y)
{
    float radius = 0.3f; // radius in tiles

    // Check X movement
    if (check_walkable_pos(cub, new_x, cub->player_pos.y, radius))
        cub->player_pos.x = new_x;
    else
        printf("invalid move x coord: %f, %f\n", cub->player_pos.x, cub->player_pos.y);

    // Check Y movement
    if (check_walkable_pos(cub, cub->player_pos.x, new_y, radius))
        cub->player_pos.y = new_y;
    else
        printf("invalid move y coord: %f, %f\n", cub->player_pos.x, cub->player_pos.y);
}

int touch(t_cub *cub, float new_x, float new_y){
    int x = new_x / cub->mlx_data.tile_size;
    int y = new_y / cub->mlx_data.tile_size;
    if (cub->map->grid[y][x] == '1')
        return 0;
    return 1;
}

void try_put_pixel(t_cub *cub, float x, float y, int color){
    if (check_screen_bounds(cub, x, y))
    {
        pixel = cub->pxl_arr
            + (y * cub->mlx_data.line_length)
            + (x * (cub->mlx_data.bits_per_pixel / 8));
        *(unsigned int *)pixel = color;
    }
}


void draw_ray(t_cub *cub, float start_x, int i){
{
    float cos_angle = cos(start_x);
    float sin_angle = sin(start_x);
    float ray_x =  cub->player_pos.x;
    float ray_y =  cub->player_pos.y;

    while(!touch(cub, ray_x, ray_y))
    {
        try_put_pixel(cub, ray_x, ray_y, 0xFF0000); //remove for 3d version
        ray_x += cos_angle;
        ray_y += sin_angle;
    }
/*
    if(!DEBUG)
    {
        float dist = fixed_dist(player->x, player->y, ray_x, ray_y, game);
        float height = (cub->mlx_data.tile_size / dist) * (WIDTH / 2);
        int start_y = (HEIGHT - height) / 2;
        int end = start_y + height;
        while(start_y < end)
        {
            try_put_pixel(i, start_y, 255, game);
            start_y++;
        }
    }*/
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