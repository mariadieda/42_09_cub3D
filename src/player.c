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

void update_pos(t_cub *cub, float new_x_px, float new_y_px)
{
    // Check X movement
    if (check_walkable_pos(cub, new_x_px, cub->player_px.y))
        cub->player_px.x = new_x_px;
    else
        printf("invalid move x coord: %f, %f\n", cub->player_tile.x, cub->player_tile.y);

    // Check Y movement
    if (check_walkable_pos(cub, cub->player_px.x, new_y_px))
        cub->player_px.y = new_y_px;
    else
        printf("invalid move y coord: %f, %f\n", cub->player_px.x, cub->player_px.y);
}

int touches_wall(t_cub *cub, int x_tile, int y_tile){

    if (cub->map->grid[y_tile][x_tile] == '1')
    {
        return 1;
    }
    /*if (y_tile > 1 && x_tile > 1 && cub->map->grid[y_tile][x_tile-1] == '1' && cub->map->grid[y_tile-1][x_tile] == '1') // blocks the diagonal peeking
        return 1;*/
    return 0;

}

void try_put_pixel(t_cub *cub, float x_px, float y_px, int color){
    char *pixel;
    int x;
    int y;
    x = (int) x_px;
    y = (int) y_px;
    if (check_screen_bounds_px(cub, x, y))
    {
        pixel = cub->pxl_arr + (y * cub->mlx_data.line_length)
            + (x * (cub->mlx_data.bits_per_pixel / 8));
        *(unsigned int *)pixel = color;
    }
}

void	put_line_pixels(t_cub *cub, t_line *line, int color)
{
    int	x_cord;
    int	y_cord;
    int	i;

    i = 0;
    while (i <= (int)line->line_len)
    {
        x_cord = (int)(line->start_point.x + (i / line->line_len) * line->x_diff);
        y_cord = (int)(line->start_point.y + (i / line->line_len) * line->y_diff);
        if (x_cord >= 0 && x_cord < cub->mlx_data.win_width && y_cord >= 0
            && y_cord < cub->mlx_data.win_height)
        {
            try_put_pixel(cub, x_cord, y_cord, color);
        }
        i++;
    }
}


void draw_line(t_cub *cub, t_pos start_pos_px, t_pos end_pos_px, int color)
{
    t_line	line;

    ft_memset(&line, 0, sizeof(t_line));
    line.start_point = start_pos_px;
    line.end_point = end_pos_px;
    line.x_diff = line.end_point.x - line.start_point.x;
    line.y_diff = line.end_point.y - line.start_point.y;
    line.line_len = sqrt(line.x_diff * line.x_diff + line.y_diff
            * line.y_diff);
    if (line.line_len > 0)
        put_line_pixels(cub, &line, color);
    /* checks wanted?
    while(!touches_wall(cub, (int)ray_x_px/cub->tile_size, (int)ray_y_px/cub->tile_size))
    {
        if (!check_map_bounds_tiles(cub, (int)ray_x_px/cub->tile_size, (int)ray_y_px/cub->tile_size))
            break;
    }*/
}

void draw_ray(t_cub *cub, float start_angle, int i){
    float cos_angle = cos(start_angle);
    float sin_angle = sin(start_angle);
    float ray_x_px =  cub->player_px.x;
    float ray_y_px =  cub->player_px.y;

    while(!touches_wall(cub, (int)ray_x_px/cub->tile_size, (int)ray_y_px/cub->tile_size))
    {
        if (!check_map_bounds_tiles(cub, (int)ray_x_px/cub->tile_size, (int)ray_y_px/cub->tile_size))
            break;
        //printf("drwaing ray from start_x:%f\n", start_angle);
        try_put_pixel(cub, ray_x_px, ray_y_px, 0xFF0000); //remove for 3d version
        ray_x_px += cos_angle;
        ray_y_px += sin_angle;
    }
    (void) i;
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
        cub->player_angle = set_rot_angle(cub->player_angle, 1);
    if (cub->move.rotate_right)
        cub->player_angle = set_rot_angle(cub->player_angle, 0);

    float cos_angle;
    float sin_angle;

    cos_angle = cos(cub->player_angle) * PLAYER_SPEED;
    sin_angle = sin(cub->player_angle) * PLAYER_SPEED;

    if (cub->move.forward)
        update_pos(cub, cub->player_px.x + cos_angle, cub->player_px.y + sin_angle);
    if (cub->move.backward)
        update_pos(cub, cub->player_px.x - cos_angle, cub->player_px.y - sin_angle);
    if (cub->move.right)
        update_pos(cub, cub->player_px.x - sin_angle, cub->player_px.y + cos_angle);
    if (cub->move.left)
        update_pos(cub, cub->player_px.x + sin_angle, cub->player_px.y - cos_angle);
}