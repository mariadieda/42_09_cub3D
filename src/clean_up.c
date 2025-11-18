#include"../inc/cub3d.h"

void    clean_up(t_cub *cub){
    {
        if (!cub)
            return ;
        if (!cub->map)
            return ;
        if (cub->map)
            {;} // todo free grid etc.
        free(cub->map);
        if (cub->img)
            mlx_destroy_image(cub->mlx, cub->img);
        if (cub->win)
            mlx_destroy_window(cub->mlx, cub->win);
        if (cub->mlx)
        {
            mlx_destroy_display(cub->mlx);
            free(cub->mlx);
        }
    }
}

void    free_array2(char **arr)
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
