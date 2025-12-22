/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 10:00:40 by mdiederi          #+#    #+#             */
/*   Updated: 2025/12/22 10:00:42 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

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
    if (i != 3){
        error = 1;
    }
    return (!error);
}

// length of nums validated befirehand:
// always has 3 parts because of validate_color called before
int	get_int_color_from_str(char **nums)
{
    int	r;
    int	g;
    int	b;
    int	color;

    r = ft_atoi(nums[0]);
    g = ft_atoi(nums[1]);
    b = ft_atoi(nums[2]);
    color = ((r << 16) | (g << 8) | b);
    return (color);
}