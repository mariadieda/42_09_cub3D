/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   populate_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:47:12 by mdiederi          #+#    #+#             */
/*   Updated: 2025/12/22 13:47:14 by mdiederi         ###   ########.fr       */
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
