/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <apavlopo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 10:00:40 by mdiederi          #+#    #+#             */
/*   Updated: 2025/12/22 14:23:07 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static void	set_fc_color(char ident, int color, t_cub *cub)
{
	if (ident == 'F')
	{
		if (cub->col->has_floor)
			error_exit(cub, "Duplicate floor color definition.\n", NULL);
		cub->col->floor = color;
		cub->col->has_floor = 1;
	}
	else if (ident == 'C')
	{
		if (cub->col->has_ceil)
			error_exit(cub, "Duplicate ceiling color definition.\n", NULL);
		cub->col->ceil = color;
		cub->col->has_ceil = 1;
	}
}

// identifier already valdiated (F or C)
void	populate_color(char *ident, t_cub *cub)
{
	char	*color_str;
	char	**nums;
	int		color;

	color_str = parse_ident_val(ident, cub->trmd_line, cub);
	nums = ft_split(color_str, ',');
	free(color_str);
	if (!nums || !validate_color(nums))
	{
		free_array(nums);
		error_exit(cub, "Invalid color format\n", NULL);
	}
	color = get_int_color_from_str(nums);
	free_array(nums);
	set_fc_color(*ident, color, cub);
	cub->header_cnt++;
}

void	validate_num(const char *s, int *error)
{
	int		j;
	char	*trimd_num;
	int		num;

	trimd_num = ft_strtrim(s, " \n\t\f\v\r");
	if (!trimd_num || trimd_num[0] == '\0')
	{
		free(trimd_num);
		*error = 1;
		return ;
	}
	j = 0;
	while (!error && trimd_num[j])
	{
		if (!ft_isdigit(trimd_num[j]))
			*error = 1;
		j++;
	}
	num = ft_atoi(trimd_num);
	free(trimd_num);
	if (!*error && (num < 0 || num > 255))
		*error = 1;
}

int	validate_color(char **nums)
{
	int	i;
	int	error;

	error = 0;
	i = 0;
	while (!error && nums[i])
	{
		validate_num(nums[i], &error);
		i++;
	}
	if (i != 3)
	{
		error = 1;
	}
	return (!error);
}

// length of nums validated befirehand:
// always has 3 parts because of validate_color called before
int	get_int_color_from_str(char **nums)
{
	int r;
	int g;
	int b;
	int color;

	r = ft_atoi(nums[0]);
	g = ft_atoi(nums[1]);
	b = ft_atoi(nums[2]);
	color = ((r << 16) | (g << 8) | b);
	return (color);
}