#include "cub3d.h"


static int rgb_to_int(t_rgb c)
{
    return ((c.r << 16) | (c.g << 8) | c.b);
}

int     ft_isspace(unsigned char c)
{
    if (c == ' ' || c == '\n' || c == '\t'
         || c == '\v' || c == '\f' || c == '\r')
        return (1);
    else 
        return (0);
}

const char* skip_ws(const char* s)
{
    while(*s && ft_isspace((unsigned char)*s))
        s++;
    return (s);
}

int     validate_color(char *s, t_cub* cub)
{
    char **nums;
    char* trimd_num;
    int num;
    int i;
    int j;

    i = 0;    
    nums = ft_split(s, ',');
    if(!nums)       // null nums
        return (0);
    while (nums[i])
        i++;
    if(i !=3)       // 200,50
    {
        free_array(nums, i);
        return (0);
    }
    i = 0;
    while(nums[i])
    {
        j = 0;
        trimd_num = ft_strtrim(nums[i], " \n\t\f\v\r");        
        if(!trimd_num)
        {
            free_array(nums, 3);
            error_exit(cub, "Error\nMalloc failed!\n", NULL);
        }
        if(trimd_num[0] == '\0') // 100,,50
        {
            free_array(nums, 3);
            free(trimd_num);
            return(0);
        }
        while(trimd_num[j])
        {
            if(trimd_num[j] < 48 ||  trimd_num[j] > 57) // "1 00" "2a1" "1!6"
            {
                free_array(nums, 3);
                free(trimd_num);
                return(0);
            }
            j++;
        }
        num = ft_atoi(trimd_num);        
        if(num < 0 || num > 255) // -1,258,1000
        {
            free_array(nums, 3);
            free(trimd_num);
            return(0);
        }
        free(trimd_num);
        i++;
    }
    free_array(nums, 3);    
    return(1);
}

t_rgb   int_color(char* s)
{
    t_rgb colors;
    char **nums;

    nums = ft_split(s, ','); //always has 3 parts because of validate_color called before
    colors.r = ft_atoi(nums[0]);
    colors.g = ft_atoi(nums[1]);
    colors.b = ft_atoi(nums[2]);
    free_array(nums, 3);
    free(s);
    return (colors);
}

char*    second_part(char *ident, char* trimd, t_cub* cub)
{
    size_t len_ident;
    const char * temp;
    char * s;
   
    len_ident = ft_strlen(ident);
   
    temp = trimd + len_ident; //find the end of the identifier and make it start of the temp
    temp = skip_ws(temp); // if there are white spaces at the beginning remove them 
   
    if(*temp == '\0')
        error_exit(cub, "Error\nMissing texture address / color!\n", (char*[]){trimd, NULL});
    s = ft_strdup(temp); // give back a malloced string
    if(!s)   
        error_exit(cub, "Error\nMalloc failed!\n", (char*[]){trimd, NULL});
    return (s);
}

char*   trim_the_line(char *line, t_cub* cub)
{
    char *trimd = ft_strtrim(line, " \n\t\f\v\r");
    if(!trimd)
        error_exit(cub, "Error\nMalloc failed!\n", (char*[]){line, NULL});
    if(*trimd == '\0')
        error_exit(cub, "Error\nEmpty line\n", (char*[]){line, trimd, NULL});
    return trimd;
}

void    populate_color(char **bufs, char *ident, int* header_cnt, t_cub* cub)
{
    char* color;
    char* trimd;

    trimd = bufs[1];
    color = second_part(ident, trimd, cub);
    if(validate_color(color, cub))
    {
        if(*ident == 'F')
        {
            if(cub->col->has_floor)
                error_exit(cub, "Error\nDouble color definition for Floor\n", (char*[]){bufs[0], trimd, color, NULL});
            cub->col->floor = int_color(color);
            cub->col->floor_color = rgb_to_int(cub->col->floor);
            cub->col->has_floor = 1;
            (*header_cnt)++;
        }
        else if(*ident == 'C')
        {
            if(cub->col->has_ceil)
                error_exit(cub, "Error\nDouble color definition for Ceil\n", (char*[]){bufs[0], trimd, color, NULL});
            cub->col->ceil = int_color(color);
            cub->col->ceil_color = rgb_to_int(cub->col->ceil);
            cub->col->has_ceil = 1;
            (*header_cnt)++;
        }      
    }        
    else    
        error_exit(cub, "Error\nInvalid Color Format\n", (char*[]){bufs[0], color, trimd, NULL});
}

int     validate_address(char* token)
{
    int i;

    i = 0;
    while(token[i])
    {
        if(ft_isspace((unsigned char)token[i]))
        {
            return (0);
        }
        i++;        
    }
    return(1);
}

int     validate_identifier(char *ident, char *trimd)
{
    size_t len_ident;
    
    len_ident = ft_strlen(ident);  
    if((ft_strlen(trimd) < len_ident))
    {
        
        return (0);
    }
    if (ft_strncmp(trimd, ident, len_ident) != 0 
        || trimd[len_ident] == '\0' || !ft_isspace((unsigned char)trimd[len_ident]))
    {
       
        return (0);
    }
    
    return(1);
}

int     populate_address(char* token, char *ident, int* header_cnt, t_cub* cub)
{       
    
    if (ft_strncmp(ident, "NO", 2) == 0)
    {
        if(cub->col->no_tex_p)       
            return (0);
        cub->col->no_tex_p = token;
        (*header_cnt)++;
    }
    else if(ft_strncmp(ident, "SO", 2) == 0)
    {
        if(cub->col->so_tex_p)
            return (0);
        cub->col->so_tex_p = token;            
        (*header_cnt)++;
    }
    else if(ft_strncmp(ident, "WE", 2) == 0)
    {
        if(cub->col->we_tex_p)
            return (0);
        cub->col->we_tex_p = token;
        (*header_cnt)++;
    }
    else if(ft_strncmp(ident, "EA", 2) == 0)
    {
        if(cub->col->ea_tex_p)
            return (0);
        cub->col->ea_tex_p = token;
        (*header_cnt)++;
    }
    return (1);    
}

void    validate_and_populate_address(char **bufs, char* idn, int* header_cnt, t_cub* cub)
{
    char* token;
    char * trimd;

    trimd = bufs[1];
    token = second_part(idn, trimd, cub);
    if(!validate_address(token))
        error_exit(cub, "Error\nInvalid Path Format\n", (char*[]){trimd, token, bufs[0], NULL});
    if (!populate_address(token, idn, header_cnt, cub))
    {
        char* s1 = "Error\nMultiple addresses for identifier: ";
        char* s2 = ft_strjoin(s1, idn);
        if(!s2)
            error_exit(cub, "Error\nMalloc failed\n", (char*[]){trimd, token, bufs[0], NULL});
        char* msg = ft_strjoin(s2, "\n");
        free(s2);
        error_exit(cub, msg, (char*[]){trimd, token, bufs[0], msg, NULL});
    }
}

void    parse_text_col_line(char *line, int* header_cnt, t_cub* cub)
{
    char* trimd;
    char *bufs[3];

    trimd = trim_the_line(line, cub);
    bufs[0] = line;
    bufs[1] = trimd;
    bufs[2] = NULL;
    if(validate_identifier("NO", trimd))
        validate_and_populate_address(bufs, "NO", header_cnt, cub);
    else if(validate_identifier("SO", trimd))
        validate_and_populate_address(bufs, "SO", header_cnt, cub);
    else if(validate_identifier("WE", trimd))
        validate_and_populate_address(bufs, "WE", header_cnt, cub);
    else if(validate_identifier("EA", trimd))
        validate_and_populate_address(bufs, "EA", header_cnt, cub);
    else if(validate_identifier("F", trimd))
        populate_color(bufs, "F", header_cnt, cub);
    else if(validate_identifier("C", trimd))
        populate_color(bufs, "C", header_cnt, cub);
    else
        error_exit(cub,  "Error\nInvalid/Missing texture/color header!\n",(char*[]){line, NULL});
    free(trimd);
}

void     check_paths_accessibility(t_cub* cub)
{
    int i = 0;
    char *paths[5] = {cub->col->no_tex_p, cub->col->so_tex_p, cub->col->we_tex_p, cub->col->ea_tex_p, NULL};
    int fd;
    while(paths[i])
    {    
        fd = open(paths[i], O_RDONLY);
        if (fd == -1)
            error_exit(cub, "Error\nCannot open texture file\n", NULL);
        char * extension = ft_strrchr(paths[i], '.');
        if(!extension || ft_strncmp(extension, ".xpm", 4) != 0)
        {
            close(fd);
            error_exit(cub, "Error\nTexture is not .xpm\n", NULL);
        }
        close(fd);
        i++;
    }    
}

void    check_col_state(t_cub* cub)
{
    if (!cub)
        error_exit(cub, "Error\nCub is not initiallized\n", NULL);
    if (cub->col == NULL) //completely NULL col
    {
        cub->col = ft_calloc(1, sizeof(*(cub->col)));
        if (!cub->col)
            error_exit(cub, "Error\nMalloc failed\n", NULL);
    }
    else if (cub->col->no_tex_p || cub->col->so_tex_p ||
            cub->col->we_tex_p || cub->col->ea_tex_p || 
            cub->col->has_floor || cub->col->has_ceil) // if col is allocated and some of the members are not empty
    {
        error_exit(cub, "Error\nInternal: col already initialized\n", NULL);
    } // if col is allocated but all members are NULL -> safe to use
}

void    check_missing_text_col(t_cub* cub, char *line)
{
    if (!cub->col->no_tex_p || !cub->col->so_tex_p ||
        !cub->col->we_tex_p || !cub->col->ea_tex_p || 
        !cub->col->has_floor || !cub->col->has_ceil)
        error_exit(cub, "Error\nMissing texture path or color\n", (char*[]){line, NULL});
}

int     is_map(char c)
{
    if (c == ' ' ||c == '1' || c == '0' || c == 'N'
         || c == 'S' || c == 'W' || c == 'E')
        return (1);
    else 
        return (0);
}

int     is_player(char c)
{
    if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
        return (1);
    else 
        return (0);
}

int     is_blank_line(const char *line)
{
    size_t i;
    size_t len;

    i = 0;
    len = ft_strlen(line);
    while (line[i] == ' '  || line[i] == '\t' 
        || line[i] == '\r' || line[i] == '\n')
        i++;
    if(len == i)
        return (1);
    else
        return (0);
}

int    validate_chars_in_map_line(char *trimd, int* has_player, int* map_started, t_cub* cub)// validate the line and start_map = 1
{
    int i;
    int trimd_len;

    i = 0;
    while(trimd[i])
    {       
        if(!is_map(trimd[i]))
            return (0);//error_exit(cub, "Error\nInvalid map character\n");
        if(is_player(trimd[i]))
        {
            if(*has_player == 1)
                return (0);//error_exit(cub, "Error\nMultiple players present\n");
            *has_player = 1;
            cub->player_pos.x = i + 0.5;
            cub->player_pos.y = cub->map->height + 0.5;
            cub->spawn_dir = trimd[i];
        }        
        i++;
    }
    if(*map_started == 0)
        *map_started = 1;
    trimd_len = ft_strlen(trimd);
    if(cub->map->width < trimd_len)
        cub->map->width = trimd_len;
    return (1);
}

void    pad_map(t_cub* cub)
{
    int y;
    int old_width;
    int add_size = 0;

    y = 0;
    while(y < cub->map->height)
    {
        old_width = ft_strlen(cub->map->grid[y]);
        add_size = cub->map->width - old_width + 1;
        cub->map->grid[y] = ft_realloc(cub->map->grid[y], old_width, (old_width+add_size));
        while(old_width < cub->map->width)
        {           
            cub->map->grid[y][old_width] = ' ';
            old_width++;
        }
        cub->map->grid[y][cub->map->width] = '\0';
        y++;
    }
}

void    allocate_map(t_cub* cub)
{   
    cub->map = ft_calloc(1, sizeof(t_map));
    if (!cub->map)
        error_exit(cub, "Error\nMalloc failed\n", NULL);

    cub->map->height = 0;
    cub->map->width = 0;
    cub->map->capacity = 8;
    cub->map->grid = malloc(sizeof(char*) * (cub->map->capacity + 1));
    if (!cub->map->grid)
        error_exit(cub, "Error\nMalloc failed for map grid\n", NULL);
}

void    add_line_to_grid(t_cub* cub, char *trimd, char* line)
{
    int i;
    char **new_grid;

    i = 0;
    if(cub->map->height == cub->map->capacity)
    {        
        new_grid = ft_calloc((cub->map->capacity*2)+1, sizeof(char*));
        if(!new_grid)
            error_exit(cub, "Error\nMalloc failed\n", (char*[]){line, trimd, NULL});
        while (i < cub->map->height)
        {
            new_grid[i] = ft_strdup(cub->map->grid[i]);
            if(!new_grid[i])
            {
                free_array(new_grid, i);        
                error_exit(cub, "Error\nMalloc failed\n", (char*[]){line, trimd, NULL});
            }
            i++;
        }
        cub->map->capacity *= 2;
        free_array(cub->map->grid, cub->map->height);
        cub->map->grid = new_grid;
    }

    cub->map->grid[cub->map->height] = ft_strdup(trimd);
    if(!cub->map->grid[cub->map->height])
        error_exit(cub, "Error\nMalloc failed\n",  (char*[]){line, trimd, NULL});
    cub->map->height++;
}

int     check_map(t_cub* cub)
{
    int y;
    int x;
    int h;
    int w;
    char **grid = cub->map->grid;

    y = 0;   
    h = cub->map->height;
    w = cub->map->width;
    while(y < cub->map->height)
    {
        x = 0;
        while (x < cub->map->width)
        {
            if((y == 0) || (y == h-1) || x == 0 || (x == w-1))
            {
                if(grid[y][x] == '0' || is_player(grid[y][x]))
                    return (0);
            }            
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
            x++;
        }
        y++;
    }
    return(1);
}

void    replace_palyer_with_floor(t_cub* cub)
{
    int x = (int)cub->player_pos.x;
    int y = (int)cub->player_pos.y;
    cub->map->grid[y][x] = '0';
}

int     parse_file(char* filename, t_cub* cub)
{
    int fd;
    char *line;
    int header_cnt;
    int map_started;
    int has_player;
    char *trimd;
    
    header_cnt = 0;
    map_started = 0;
    has_player = 0;
    fd = open(filename, O_RDONLY);
    if (fd == -1)
        error_exit(cub, "Error\nError opening file\n", NULL);
    check_col_state(cub);
    allocate_map(cub);
    while(1)
    {
        line = get_next_line(fd);
        if (!line)
            break;
        
        if(header_cnt < 6)
        {
            if(is_blank_line(line))
            {
                free(line);
                continue;
            }
            parse_text_col_line(line, &header_cnt, cub);
        }
        else 
        {
            if (header_cnt == 6)
            {
                check_missing_text_col(cub, line);
                header_cnt++;
            }
            if (!map_started && is_blank_line(line))
            {   
                free(line);
                continue;                
            }
            else
            {               
                if(is_blank_line(line))               
                    error_exit(cub, "Error\nInvalid blank line inside map\n", (char*[]){line, NULL});
                trimd = ft_strtrim(line, "\r\n");
                if(!validate_chars_in_map_line(trimd, &has_player, &map_started, cub))
                    error_exit(cub, "Error\nInvalid map character or Multiple players present\n", (char*[]){trimd, line, NULL});
                add_line_to_grid(cub, trimd, line);          
                free(trimd);
            }
        }
        free(line);
    } 
    cub->map->grid[cub->map->height] = NULL;
    if(!map_started)
        error_exit(cub, "Error\nNo map found\n", NULL);
    if(has_player == 0)
        error_exit(cub, "Error\nNo player found\n", NULL);
    pad_map(cub);
    close(fd);
    check_paths_accessibility(cub);
    if (!check_map(cub))
        error_exit(cub, "Error\nInvalid map structure\n", NULL);
    replace_palyer_with_floor(cub);

    return(1);
}

/*
No need to close(fd) when error_exit() because on exit() kernel does clean it up.
*/
