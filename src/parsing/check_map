/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <apavlopo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 10:17:44 by mdiederi          #+#    #+#             */
/*   Updated: 2025/12/22 11:13:32 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int     check_tile(int y, int x, char **grid, t_cub* cub)
{
    int h;
    int w;

    h = cub->map->height;
    w = cub->map->width;
    if((y == 0) || (y == h-1) || x == 0 || (x == w-1))
        if(grid[y][x] == '0' || is_player(grid[y][x]))
            return (0);
    else if(grid[y][x] == '0'|| is_player(grid[y][x]))
    {
        if((grid[y-1][x] != '1' &&  grid[y-1][x] != '0' 
            &&  !is_player(grid[y-1][x])))
                return (0);
        if((grid[y][x-1] != '1' &&  grid[y][x-1] != '0' 
            &&  !is_player(grid[y][x-1])))
                return (0);
        if((grid[y+1][x] != '1' &&  grid[y+1][x] != '0' 
            &&  !is_player(grid[y+1][x])))
                return (0);
        if((grid[y][x+1] != '1' &&  grid[y][x+1] != '0' 
            &&  !is_player(grid[y][x+1])))
                return (0);
    }
    return (1);
}


int     check_map(t_cub* cub)
{
    int y;
    int x;
    int h;
    int w;
    char **grid = cub->map->grid;

    y = 0;
    if (cub->map->width <= 0 || cub->map->height <= 0)
        error_exit(cub,"Invalid map size\n", NULL);
    h = cub->map->height;
    w = cub->map->width;
    while(y < cub->map->height)
    {
        x = 0;
        while (x < cub->map->width)
        {
            if(!check_tile(y, x, grid, cub))
                return (0);
            x++;
        }
        y++;
    }
    return(1);
}
