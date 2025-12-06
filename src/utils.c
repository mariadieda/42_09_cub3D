#include "../inc/cub3d.h"

double ft_fmin(double x, double y)
{
    if (x <= y)
        return x;
    return y;
}

void print_map(t_cub *cub){
    int i = 0;
    while(cub->map->grid[i])
    {
        printf("\n%s\n\n",cub->map->grid[i]);
        i++;
    }
}