#include"../inc/cub3d.h"

void    free_array(char **arr, int grid_height)
{
    int i;
   
    i = 0;
    if (!arr) 
        return;
    while(i < grid_height)
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

void    clean_up(t_cub *cub){
    {
        if (!cub)
            return ;        
        if (cub->img)
            mlx_destroy_image(cub->mlx, cub->img);
        if (cub->win)
            mlx_destroy_window(cub->mlx, cub->win);
        if (cub->mlx)
        {
            mlx_destroy_display(cub->mlx);
            free(cub->mlx);
        }
        if (cub->map)
        {
            free_array(cub->map->grid, cub->map->height);
            free(cub->map);        
        }
        if(cub->col)
        {
            if(cub->col->no_tex_p)
                free(cub->col->no_tex_p);
            if(cub->col->so_tex_p)
                free(cub->col->so_tex_p);
            if(cub->col->we_tex_p)
                free(cub->col->we_tex_p);
            if(cub->col->ea_tex_p)
                free(cub->col->ea_tex_p);           
            free(cub->col);
        }
    }
}

void    free_array2(char **arr) //only for usage when calling with array on stack: error_exit(cub, "Error\nMalloc failed!\n", (char*[]){trimd, NULL});
{
    int i;
   
    i = 0;
    if (!arr)
        return;
    while(arr[i])
    {
        free(arr[i]);
        i++;
    }
}

void	error_exit(t_cub *cub, char *msg, char** to_be_freed)
{
    ft_putstr_fd(msg, 2);    
    free_array2(to_be_freed);
    clean_up(cub);
    exit(1);
}
