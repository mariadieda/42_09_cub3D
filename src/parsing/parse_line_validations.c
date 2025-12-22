/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line_validations.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:57:25 by mdiederi          #+#    #+#             */
/*   Updated: 2025/12/22 13:57:29 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

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

	token = parse_ident_val(idn, cub->trmd_line, cub);
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
	paths[4] = NULL;
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

// validate the line and start_map = 1
void	validate_line_chars_map(t_cub *cub, int *has_player, int *map_started)
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
