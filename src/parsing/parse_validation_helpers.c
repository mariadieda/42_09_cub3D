// todo add header

#include "inc/cub3d.h"

int	is_blank_line(const char *line)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(line);
	while (ft_is_in_set(line[i], " \n\t\v\f\r") && i < len)
		i++;
	if (len == i)
		return (1);
	else
		return (0);
}

int	is_player(char c)
{
	if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
		return (1);
	else
		return (0);
}

void	check_missing_text_col(t_cub *cub)
{
	if (!cub->col->no_tex_p || !cub->col->so_tex_p || !cub->col->we_tex_p
		|| !cub->col->ea_tex_p || !cub->col->has_floor || !cub->col->has_ceil)
		error_exit(cub, "Missing texture path or color\n", NULL);
	cub->header_cnt++;
}

int	validate_address(char *token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		if (ft_is_in_set(token[i], " \n\t\v\f\r"))
		{
			return (0);
		}
		i++;
	}
	return (1);
}

int	validate_identifier(char *ident, char *trimd)
{
	size_t	len_ident;

	len_ident = ft_strlen(ident);
	if ((ft_strlen(trimd) < len_ident))
		return (0);
	if (ft_strncmp(trimd, ident, len_ident) != 0 || trimd[len_ident] == '\0'
		|| !ft_is_in_set(trimd[len_ident], " \n\t\v\f\r"))
	{
		return (0);
	}
	return (1);
}
