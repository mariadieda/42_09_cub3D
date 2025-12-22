/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:17:44 by mdiederi          #+#    #+#             */
/*   Updated: 2025/12/16 10:17:48 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	add_line_to_grid(t_cub *cub, char *trimd)
{
	if (cub->map->height == cub->map->capacity)
		cub->map->grid = expand_grid(cub);
	cub->map->grid[cub->map->height] = ft_strdup(trimd);
	if (!cub->map->grid[cub->map->height])
		error_exit(cub, "Malloc failed\n", NULL);
	cub->map->height++;
}

int	inside_header(t_cub *cub, int map_started)
{
	if (cub->header_cnt < 6)
	{
		if (is_blank_line(cub->cur_line))
		{
			free(cub->cur_line);
			return (1);
		}
		parse_text_col_line(cub);
		free_tmp_lines(cub);
		return (1);
	}
	if (cub->header_cnt == 6)
		check_missing_text_col(cub);
	if (!map_started && is_blank_line(cub->cur_line))
	{
		free(cub->cur_line);
		return (1);
	}
	return (0);
}

void	finalize_map(t_cub *cub, int fd, int map_started, int has_player)
{
	cub->map->grid[cub->map->height] = NULL;
	if (!map_started)
		error_exit(cub, "No map found\n", NULL);
	if (has_player == 0)
		error_exit(cub, "No player found\n", NULL);
	pad_map(cub);
	close(fd);
	check_texture_paths_accessibility(cub);
	if (!check_map(cub))
		error_exit(cub, "Invalid map structure\n", NULL);
	cub->map->grid[(int)cub->player_tile.y][(int)cub->player_tile.x] = '0';
}

void	loop_through_lines(t_cub *cub, int fd, int *map_started,
		int *has_player)
{
	while (1)
	{
		cub->cur_line = get_next_line(fd);
		if (!cub->cur_line)
			break ;
		if (inside_header(cub, *map_started))
			continue ;
		if (is_blank_line(cub->cur_line))
			error_exit(cub, "Invalid blank line inside map\n", NULL);
		cub->trmd_line = ft_strtrim(cub->cur_line, "\r\n");
		if (!cub->trmd_line)
			error_exit(cub, "Malloc failed\n", NULL);
		validate_chars_in_map_line(cub, has_player, map_started);
		add_line_to_grid(cub, cub->trmd_line);
		free_tmp_lines(cub);
	}
}

int	parse_file(char *filename, t_cub *cub)
{
	int	fd;
	int	map_started;
	int	has_player;

	map_started = 0;
	has_player = 0;
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		error_exit(cub, "Error opening file\n", NULL);
	check_col_state(cub);
	allocate_map(cub);
	loop_through_lines(cub, fd, &map_started, &has_player);
	finalize_map(cub, fd, map_started, has_player);
	return (1);
}

/*
No need to close(fd) when error_exit() because on exit() kernel does clean it up.
*/
