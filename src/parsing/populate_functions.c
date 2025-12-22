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
char	*parse_ident_val(char *ident, char *trimd, t_cub *cub)
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

static int	set_texture(char *token, char *texture, t_cub *cub)
{
	if (texture)
		return (0);
	else
	{
		texture = token;
		cub->header_cnt++;
		return (1);
	}
}

int	populate_address(char *token, char *ident, t_cub *cub)
{
	if (ft_strncmp(ident, "NO", 2) == 0)
		return (set_texture(token, cub->col->no_tex_p, cub));
	else if (ft_strncmp(ident, "SO", 2) == 0)
		return (set_texture(token, cub->col->so_tex_p, cub));
	else if (ft_strncmp(ident, "WE", 2) == 0)
		return (set_texture(token, cub->col->we_tex_p, cub));
	else if (ft_strncmp(ident, "EA", 2) == 0)
		return (set_texture(token, cub->col->ea_tex_p, cub));
	return (1);
}