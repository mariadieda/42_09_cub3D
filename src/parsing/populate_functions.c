/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   populate_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <apavlopo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:47:12 by mdiederi          #+#    #+#             */
/*   Updated: 2025/12/22 14:23:24 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

const char	*skip_ws(const char *s)
{
	while (*s && ft_is_in_set(*s, " \n\t\v\f\r"))
		s++;
	return (s);
}

// find the end of the identifier and make it start of the temp
// if there are white spaces at the beginning remove them
// give back a malloced string
char	*parse_ident_val(char *ident, char *trimd, t_cub *cub)
{
	size_t		len_ident;
	const char	*temp;
	char		*s;

	len_ident = ft_strlen(ident);
	temp = trimd + len_ident;
	temp = skip_ws(temp);
	if (*temp == '\0')
		error_exit(cub, "Missing texture address/color!\n", NULL);
	s = ft_strdup(temp);
	if (!s)
		error_exit(cub, "Malloc failed!\n", NULL);
	return (s);
}

static int	set_texture(char *token, char **texture_ptr, t_cub *cub)
{
	if (*texture_ptr)
		return (0);
	*texture_ptr = token;
	cub->header_cnt++;
	return (1);
}

int	populate_address(char *token, char *ident, t_cub *cub)
{
	if (ft_strncmp(ident, "NO", 2) == 0)
		return (set_texture(token, &cub->col->no_tex_p, cub));
	else if (ft_strncmp(ident, "SO", 2) == 0)
		return (set_texture(token, &cub->col->so_tex_p, cub));
	else if (ft_strncmp(ident, "WE", 2) == 0)
		return (set_texture(token, &cub->col->we_tex_p, cub));
	else if (ft_strncmp(ident, "EA", 2) == 0)
		return (set_texture(token, &cub->col->ea_tex_p, cub));
	return (1);
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
