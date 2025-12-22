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



char	*trim_the_line(char *line, t_cub *cub)
{
	char	*trimd;

	trimd = ft_strtrim(line, " \n\t\f\v\r");
	if (!trimd)
		error_exit(cub, "Malloc failed!\n", NULL);
	if (*trimd == '\0')
	{
		free(trimd);
		error_exit(cub, "Empty line\n", NULL);
	}
	return (trimd);
}


void	validate_and_populate_address(char *idn, t_cub *cub)
{
	char	*token;
	char	*s1;
	char	*s2;
	char	*msg;

	token = second_part(idn, cub->trmd_line, cub);
	if (!validate_address(token))
		error_exit(cub, "Invalid Path Format\n", (char *[]){token, NULL});
	if (!populate_address(token, idn, cub))
	{
		s1 = "Multiple addresses for identifier: ";
		s2 = ft_strjoin(s1, idn);
		if (!s2)
			error_exit(cub, "Malloc failed\n", (char *[]){token, NULL});
		msg = ft_strjoin(s2, "\n");
		free(s2);
		error_exit(cub, msg, (char *[]){token, msg, NULL});
	}
}

void	parse_text_col_line(t_cub *cub)
{
	cub->trmd_line = trim_the_line(cub->cur_line, cub);
	if (validate_identifier("NO", cub->trmd_line))
		validate_and_populate_address("NO", cub);
	else if (validate_identifier("SO", cub->trmd_line))
		validate_and_populate_address("SO", cub);
	else if (validate_identifier("WE", cub->trmd_line))
		validate_and_populate_address("WE", cub);
	else if (validate_identifier("EA", cub->trmd_line))
		validate_and_populate_address("EA", cub);
	else if (validate_identifier("F", cub->trmd_line))
		populate_color("F", cub);
	else if (validate_identifier("C", cub->trmd_line))
		populate_color("C", cub);
	else
		error_exit(cub, "Invalid/missing texture/color header!\n", NULL);
}

void	check_texture_paths_accessibility(t_cub *cub)
{
	int		i;
	char	*paths[5];
	int		fd;
	char	*extension;

	paths[0] = cub->col->no_tex_p;
	paths[1] = cub->col->so_tex_p;
	paths[2] = cub->col->we_tex_p;
	paths[3] = cub->col->ea_tex_p;
	i = 0;
	while (paths[i])
	{
		fd = open(paths[i], O_RDONLY);
		if (fd == -1)
			error_exit(cub, "Cannot open texture file\n", NULL);
		extension = ft_strrchr(paths[i], '.');
		if (!extension || ft_strncmp(extension, ".xpm", 4) != 0)
		{
			close(fd);
			error_exit(cub, "Texture is not .xpm\n", NULL);
		}
		close(fd);
		i++;
	}
}

void	check_col_state(t_cub *cub)
{
	if (!cub)
		error_exit(cub, "Cub is not initialized\n", NULL);
	if (cub->col == NULL)
	{
		cub->col = ft_calloc(1, sizeof(*(cub->col)));
		if (!cub->col)
			error_exit(cub, "Malloc failed\n", NULL);
	}
	else if (cub->col->no_tex_p || cub->col->so_tex_p || cub->col->we_tex_p
			|| cub->col->ea_tex_p || cub->col->has_floor || cub->col->has_ceil)
	{
		error_exit(cub, "Internal: col already initialized\n", NULL);
	}
}

// validate the line and start_map = 1
void validate_chars_in_map_line(t_cub *cub, int *has_player, int *map_started)
{
	int	i;
	int	trimd_len;

	i = 0;
	while (cub->trmd_line[i])
	{
		if (!ft_is_in_set(cub->trmd_line[i], " 10NSWE"))
			error_exit(cub, "Invalid map character\n", NULL);
		if (is_player(cub->trmd_line[i]))
		{
			if (*has_player == 1)
				error_exit(cub, "Multiple players present\n", NULL);
			*has_player = 1;
			cub->player_tile.x = i + 0.5;
			cub->player_tile.y = cub->map->height + 0.5;
			cub->spawn_dir = cub->trmd_line[i];
		}
		i++;
	}
	if (*map_started == 0)
		*map_started = 1;
	trimd_len = ft_strlen(cub->trmd_line);
	if (cub->map->width < trimd_len)
		cub->map->width = trimd_len;
}

void	add_line_to_grid(t_cub *cub, char *trimd, char *line)
{
	int		i;
	char	**new_grid;

	i = 0;
	if (cub->map->height == cub->map->capacity)
	{
		new_grid = ft_calloc((cub->map->capacity * 2) + 1, sizeof(char *));
		if (!new_grid)
			error_exit(cub, "Malloc failed\n", (char *[]){line, trimd, NULL});
		while (i < cub->map->height)
		{
			new_grid[i] = ft_strdup(cub->map->grid[i]);
			if (!new_grid[i])
			{
				free_n_array(new_grid, i);
				error_exit(cub, "Malloc failed\n", (char *[]){line, trimd,
						NULL});
			}
			i++;
		}
		cub->map->capacity *= 2;
		free_n_array(cub->map->grid, cub->map->height);
		cub->map->grid = new_grid;
	}
	cub->map->grid[cub->map->height] = ft_strdup(trimd);
	if (!cub->map->grid[cub->map->height])
		error_exit(cub, "Malloc failed\n", (char *[]){line, trimd, NULL});
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
	while (1)
	{
		cub->cur_line = get_next_line(fd);
		if (!cub->cur_line)
			break ;
		if (inside_header(cub, map_started))
			continue ;
		if (is_blank_line(cub->cur_line))
			error_exit(cub, "Invalid blank line inside map\n", NULL);
		cub->trmd_line = ft_strtrim(cub->cur_line, "\r\n");
		if (!cub->trmd_line)
			error_exit(cub, "Malloc failed\n", NULL);
		validate_chars_in_map_line(cub, &has_player, &map_started);
		add_line_to_grid(cub, cub->trmd_line, cub->cur_line);
		free_tmp_lines(cub);
	}
	finalize_map(cub, fd, map_started, has_player);
	return (1);
}

/*
No need to close(fd) when error_exit() because on exit() kernel does clean it up.
*/
