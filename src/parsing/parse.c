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

const char	*skip_ws(const char *s)
{
	while (*s && ft_is_in_set(*s, " \n\t\v\f\r"))
		s++;
	return (s);
}

// find the end of the identifier and make it start of the temp
// if there are white spaces at the beginning remove them
// give back a malloced string
char	*second_part(char *ident, char *trimd, t_cub *cub)
{
	size_t		len_ident;
	const char	*temp;
	char		*s;

	len_ident = ft_strlen(ident);
	temp = trimd + len_ident;
	temp = skip_ws(temp);
	if (*temp == '\0')
		error_exit(cub, "Missing texture address/color!\n", (char *[]){trimd,
				NULL});
	s = ft_strdup(temp);
	if (!s)
		error_exit(cub, "Malloc failed!\n", (char *[]){trimd, NULL});
	return (s);
}

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

// identifier already valdiated (F or C)
void	populate_color(char *ident, t_cub *cub)
{
	char	*color_str;
	char	**nums;
	int		color;

	color_str = second_part(ident, cub->trmd_line, cub);
	nums = ft_split(color_str, ',');
	free(color_str);
	if (!nums || !validate_color(nums))
	{
		free_array(nums);
		error_exit(cub, "Invalid color format\n", NULL);
	}
	color = get_int_color_from_str(nums);
	free_array(nums);
	if (*ident == 'F')
	{
		if (cub->col->has_floor)
			error_exit(cub, "Duplicate floor color definition.\n", NULL);
		cub->col->floor = color;
		cub->col->has_floor = 1;
	}
	else if (*ident == 'C')
	{
		if (cub->col->has_ceil)
			error_exit(cub, "Duplicate ceiling color definition.\n", NULL);
		cub->col->ceil = color;
		cub->col->has_ceil = 1;
	}
	cub->header_cnt++;
}

int	populate_address(char *token, char *ident, t_cub *cub)
{
	if (ft_strncmp(ident, "NO", 2) == 0)
	{
		if (cub->col->no_tex_p)
			return (0);
		cub->col->no_tex_p = token;
	}
	else if (ft_strncmp(ident, "SO", 2) == 0)
	{
		if (cub->col->so_tex_p)
			return (0);
		cub->col->so_tex_p = token;
	}
	else if (ft_strncmp(ident, "WE", 2) == 0)
	{
		if (cub->col->we_tex_p)
			return (0);
		cub->col->we_tex_p = token;
	}
	else if (ft_strncmp(ident, "EA", 2) == 0)
	{
		if (cub->col->ea_tex_p)
			return (0);
		cub->col->ea_tex_p = token;
	}
	cub->header_cnt++;
	return (1);
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
	if (cub->col == NULL) // completely NULL col
	{
		cub->col = ft_calloc(1, sizeof(*(cub->col)));
		if (!cub->col)
			error_exit(cub, "Malloc failed\n", NULL);
	}
	else if (cub->col->no_tex_p || cub->col->so_tex_p || cub->col->we_tex_p
			|| cub->col->ea_tex_p || cub->col->has_floor || cub->col->has_ceil)
	// if col is allocated and some of the members are not empty
	{
		error_exit(cub, "Internal: col already initialized\n", NULL);
	} // if col is allocated but all members are NULL -> safe to use
}

int	validate_chars_in_map_line(char *trimd, int *has_player, int *map_started,
		t_cub *cub)
// validate the line and start_map = 1
{
	int i;
	int trimd_len;

	i = 0;
	while (trimd[i])
	{
		if (!ft_is_in_set(trimd[i], " 10NSWE"))
			return (0); // error_exit(cub, "Invalid map character\n");
		if (is_player(trimd[i]))
		{
			if (*has_player == 1)
				return (0); // error_exit(cub, "Multiple players present\n");
			*has_player = 1;
			cub->player_tile.x = i + 0.5;
			cub->player_tile.y = cub->map->height + 0.5;
			cub->spawn_dir = trimd[i];
		}
		i++;
	}
	if (*map_started == 0)
		*map_started = 1;
	trimd_len = ft_strlen(trimd);
	if (cub->map->width < trimd_len)
		cub->map->width = trimd_len;
	return (1);
}

void	pad_map(t_cub *cub)
{
	int	y;
	int	old_width;
	int	add_size;

	add_size = 0;
	y = 0;
	while (y < cub->map->height)
	{
		old_width = ft_strlen(cub->map->grid[y]);
		add_size = cub->map->width - old_width + 1;
		cub->map->grid[y] = ft_realloc(cub->map->grid[y], old_width, (old_width
					+ add_size));
		while (old_width < cub->map->width)
		{
			cub->map->grid[y][old_width] = ' ';
			old_width++;
		}
		cub->map->grid[y][cub->map->width] = '\0';
		y++;
	}
}

void	allocate_map(t_cub *cub)
{
	cub->map = ft_calloc(1, sizeof(t_map));
	if (!cub->map)
		error_exit(cub, "Malloc failed\n", NULL);
	cub->map->height = 0;
	cub->map->width = 0;
	cub->map->capacity = 8;
	cub->map->grid = malloc(sizeof(char *) * (cub->map->capacity + 1));
	if (!cub->map->grid)
		error_exit(cub, "Malloc failed for map grid\n", NULL);
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
		free(cub->trmd_line);
		cub->trmd_line = NULL;
		free(cub->cur_line);
		cub->cur_line = NULL;
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
		if (!validate_chars_in_map_line(cub->trmd_line, &has_player,
				&map_started, cub))
			error_exit(cub,
						"Invalid map character or multiple players present\n",
						NULL);
		add_line_to_grid(cub, cub->trmd_line, cub->cur_line);
		free(cub->trmd_line);
		free(cub->cur_line);
		cub->trmd_line = NULL;
		cub->cur_line = NULL;
	}
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
	return (1);
}

/*
No need to close(fd) when error_exit() because on exit() kernel does clean it up.
*/
