/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:16:45 by mdiederi          #+#    #+#             */
/*   Updated: 2025/12/16 10:16:49 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

float set_player_spawn_dir(const char c)
{
	int i;
	char dirs[5];

	dirs[0] = 'E';
	dirs[1] = 'S';
	dirs[2] = 'W';
	dirs[3] = 'N';
	dirs[4] = '\0';
	i = 0;
	while (dirs[i] != '\0')
	{
		printf("dir: %c i = %d\n", dirs[i], i);
		if (dirs[i] == c)
			return ((float)(i * M_PI_2));
		i++;
	}
	return (0);
}

float	set_rot_angle(float current_angle, int decrease)
{
	float	rotate_speed;
	float	angle;

	angle = current_angle;
	rotate_speed = PLAYER_ROTATE_SPEED;
	if (decrease)
		rotate_speed *= -1.f;
	angle += rotate_speed;
	if (angle > (float)(2.f * M_PI))
		angle = 0.f;
	if (angle < 0.f)
		angle = (float)(2.f * M_PI);
	return (angle);
}

// Check X and Y movement separately (allows sliding along walls)
void	update_pos(t_cub *cub, float new_x_px, float new_y_px)
{
	if (check_walkable_pos(cub, new_x_px, cub->player_px.y))
		cub->player_px.x = new_x_px;
	if (check_walkable_pos(cub, cub->player_px.x, new_y_px))
		cub->player_px.y = new_y_px;
}
/* DEBUG collisions with:
*	else
		printf("invalid move x coord: %f, %f\n", cub->player_tile.x,
				cub->player_tile.y);
 *
* 	else
		printf("invalid move y coord: %f, %f\n", cub->player_px.x,
				cub->player_px.y);
 */

int	touches_wall(t_cub *cub, int x_tile, int y_tile)
{
	if (cub->map->grid[y_tile][x_tile] == '1')
		return (1);
	return (0);
}

void	try_put_pixel(t_cub *cub, float x_px, float y_px, int color)
{
	char	*pixel;
	int		x;
	int		y;

	if (check_screen_bounds_px(cub, x_px, y_px))
	{
		x = (int)x_px;
		y = (int)y_px;
		pixel = cub->pxl_arr + (y * cub->mlx_data.line_length) + (x
				* cub->mlx_data.bytes_per_pixel);
		*(unsigned int *)pixel = color;
	}
}

void	put_line_pixels(t_cub *cub, t_line *line, int color)
{
	float	x_cord;
	float	y_cord;
	int	i;

	i = 0;
	while (i <= (int)line->line_len)
	{
		x_cord = (float)(line->start_point.x + (i / line->line_len)
				* line->x_diff);
		y_cord = (float)(line->start_point.y + (i / line->line_len)
				* line->y_diff);
		if (x_cord >= 0 && x_cord < (float)cub->mlx_data.win_width && y_cord >= 0
			&& y_cord < (float)cub->mlx_data.win_height)
		{
			try_put_pixel(cub, x_cord, y_cord, color);
		}
		i++;
	}
}

void	draw_line(t_cub *cub, t_pos start_pos_px, t_pos end_pos_px, int color)
{
	t_line	line;

	ft_memset(&line, 0, sizeof(t_line));
	line.start_point = start_pos_px;
	line.end_point = end_pos_px;
	line.x_diff = line.end_point.x - line.start_point.x;
	line.y_diff = line.end_point.y - line.start_point.y;
	line.line_len = sqrt(line.x_diff * line.x_diff + line.y_diff * line.y_diff);
	if (line.line_len > 0)
		put_line_pixels(cub, &line, color);
}

// rotation = viewer perspective, not actual position change
// rotation should always be possible
void	player_move(t_cub *cub)
{
	float	cos_angle;
	float	sin_angle;
	float	x;
	float	y;

	if (cub->move.rotate_left)
		cub->player_angle = set_rot_angle(cub->player_angle, 1);
	if (cub->move.rotate_right)
		cub->player_angle = set_rot_angle(cub->player_angle, 0);
	cos_angle = cosf(cub->player_angle) * PLAYER_SPEED;
	sin_angle = sinf(cub->player_angle) * PLAYER_SPEED;
	x = cub->player_px.x;
	y = cub->player_px.y;
	if (cub->move.forward)
		update_pos(cub, x + cos_angle, y + sin_angle);
	if (cub->move.backward)
		update_pos(cub, x - cos_angle, y - sin_angle);
	if (cub->move.right)
		update_pos(cub, x - sin_angle, y + cos_angle);
	if (cub->move.left)
		update_pos(cub, x + sin_angle, y - cos_angle);
}
