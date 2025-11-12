#include "cub3d.h"

void free_split(char **arr)
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
    free(arr);
}

int ft_isspace(unsigned char c)
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

int validate_color(char *s, t_cub* cub)
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
        free_split(nums);
        return (0);
    }
    i = 0;       
    while(nums[i])
    {
        j = 0;
        trimd_num = ft_strtrim(nums[i], " \n\t\f\v\r");        
        if(!trimd_num)
        {
            free_split(nums);
            error_exit(cub, "Error\nMalloc failed!\n");
        }
        if(trimd_num[0] == '\0') // 100,,50
        {
            free_split(nums);
            free(trimd_num);
            return(0);
        }
        while(trimd_num[j])
        {
            if(trimd_num[j] < 48 ||  trimd_num[j] > 57) // "1 00" "2a1" "1!6"
            {
                free_split(nums);
                free(trimd_num);
                return(0);
            }
            j++;
        }
        num = ft_atoi(trimd_num);        
        if(num < 0 || num > 255) // -1,258,1000
        {
            free_split(nums);
            free(trimd_num);
            return(0);
        }
        free(trimd_num);
        i++;
    }
    free_split(nums);    
    return(1);
}

t_rgb int_color(char* s)
{
    t_rgb colors;
    char **nums;

    nums = ft_split(s, ','); //always has 3 parts because of validate_color called before
    colors.r = ft_atoi(nums[0]);
    colors.g = ft_atoi(nums[1]);
    colors.b = ft_atoi(nums[2]);
    free_split(nums);
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
    {
        free(trimd);
        error_exit(cub, "Error\nMissing texture address / color!\n");
    }        
    s = ft_strdup(temp); // give back a malloced string
    if(!s)
    {
        free(trimd);
        error_exit(cub, "Error\nMalloc failed!\n");
    }
    return (s);
}

void    populate_color(char* trimd, char *ident, int* header_cnt, t_cub* cub)
{
    char* color;

    color = second_part(ident, trimd, cub);   
    if(validate_color(color, cub))
    {
        if(*ident == 'F')
        {
            cub->col->floor = int_color(color);
            cub->col->has_floor = 1;      
            (*header_cnt)++;
        }
        else if(*ident == 'C')
        {
            cub->col->ceil = int_color(color);
            cub->col->has_ceil = 1;
            (*header_cnt)++;
        }      
    }        
    else
    {
        free(color);
        free(trimd);
        error_exit(cub, "Error\nInvalid Color Format\n");
    }
}

int validate_address(char* token)
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

int validate_identifier(char *trimd, char *ident)
{
    size_t len_ident;
    
    if(!trimd || !ident) // trimd can't be null because already checked
        return (0);

    len_ident = ft_strlen(ident);  
    if((ft_strlen(trimd) < len_ident))
        return(0);
    if (ft_strncmp(trimd, ident, len_ident) != 0 
        || trimd[len_ident] == '\0' || !ft_isspace((unsigned char)trimd[len_ident]))
        return (0);

    return(1);
}

void populate_address(char* trimd, char *ident, int* header_cnt, t_cub* cub)
{
    char* token = second_part(ident, trimd, cub);
    if(!validate_address(token))
    {
        free(trimd);
        free(token);
        error_exit(cub, "Error\nInvalid Path Format\n");
    }        
    else
    {
        if (ft_strncmp(ident, "NO", 2) == 0)
        {
            cub->col->no_tex_p = token;
            (*header_cnt)++;
        }
        else if(ft_strncmp(ident, "SO", 2) == 0)
        {
            cub->col->so_tex_p = token;
            (*header_cnt)++;
        }
        else if(ft_strncmp(ident, "WE", 2) == 0)
        {
            cub->col->we_tex_p = token;
            (*header_cnt)++;
        }
        else if(ft_strncmp(ident, "EA", 2) == 0)
        {
            cub->col->ea_tex_p = token;
            (*header_cnt)++;
        }
    }    
}

void    parse_text_col_line(char *line, int* header_cnt, t_cub* cub)
{
    char *trimd = ft_strtrim(line, " \n\t\f\v\r");
    if(!trimd)
        error_exit(cub, "Error\nMalloc failed!\n");
    if(*trimd == '\0')
        error_exit(cub, "Error\nEmpty line\n");

    if(validate_identifier(trimd, "NO"))
        populate_address(trimd, "NO", header_cnt, cub);
    else if(validate_identifier(trimd, "SO"))
        populate_address(trimd, "SO", header_cnt, cub);
    else if(validate_identifier(trimd, "WE"))   
        populate_address(trimd, "WE", header_cnt, cub);
    else if(validate_identifier(trimd, "EA"))
        populate_address(trimd, "EA", header_cnt, cub);
    else if(validate_identifier(trimd, "F"))
        populate_color(trimd, "F", header_cnt, cub);        
    else if(validate_identifier(trimd, "C"))
        populate_color(trimd, "C", header_cnt, cub);
    else
    {
        //ft_putstr_fd(trimd, 1);
        error_exit(cub,  "Error\nInvalid/Missing texture/color header!\n");
    }
        
    free(trimd);
}

int check_paths_accessibility(t_cub* cub)
{
    int i = 0;
    char *paths[5] = {cub->col->no_tex_p, cub->col->so_tex_p, cub->col->we_tex_p, cub->col->ea_tex_p, NULL};
    int fd;
    while(paths[i])
    {    
        fd = open(paths[i], O_RDONLY);
        if (fd == -1)
            return (0);
        close(fd);
        i++;
    }
    return(1);   
}

void check_col_state(t_cub* cub)
{
    if (cub->col == NULL) //completely NULL col
    {
        cub->col = ft_calloc(1, sizeof(*(cub->col)));
        if (!cub->col)
            error_exit(cub, "Error\nMalloc failed\n");
    }
    else if (cub->col->no_tex_p || cub->col->so_tex_p ||
            cub->col->we_tex_p || cub->col->ea_tex_p) // if col is allocated and some of the members are not empty
    {
        error_exit(cub, "Error\nInternal: col already initialized\n");
    } // if col is allocated but all members are NULL -> safe to use
}

void check_missing_text_col(t_cub* cub)
{
    if (!cub->col->no_tex_p || !cub->col->so_tex_p ||
        !cub->col->we_tex_p || !cub->col->ea_tex_p || 
        !cub->col->has_floor || !cub->col->has_ceil)
        error_exit(cub, "Error\nMissing or multiple texture path(s) or color\n"); //in case i.e. 6 valid kheaders + 1 duplicate valid header
}

int is_map(char c)
{
    if (c == ' ' ||c == '1' || c == '0' || c == 'N'
         || c == 'S' || c == 'W' || c == 'E')
        return (1);
    else 
        return (0);
}

int is_player(char c)
{
    if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
        return (1);
    else 
        return (0);
}

void validate_chars_in_map_line(char *trimd, int* has_player, int* map_started, t_cub* cub)// validate the line and start_map = 1
{
    int i;
    int trimd_len;

    i = 0;    
    //ft_printf("Entered in validate_chars_in_map_line");
    while(trimd[i])
    {       
        if(!is_map(trimd[i]))
            error_exit(cub, "Error\nInvalid map character\n");
        if(is_player(trimd[i]))
        {
            if(*has_player == 1)
                error_exit(cub, "Error\nMultiple players present\n");
            *has_player = 1;
            cub->player_pos.x = i;
            cub->player_pos.y = cub->map->height;
            //ft_printf("cub->player_pos.x: %i", cub->player_pos.x);
           // ft_printf("cub->player_pos.y: %i", cub->player_pos.y);
        }        
        i++;
    }
    if(*map_started == 0)
        *map_started = 1;
    trimd_len = ft_strlen(trimd);
    if(cub->map->width < trimd_len)
        cub->map->width = trimd_len;
    cub->map->height++;
}

void    pad_map(t_cub* cub) //to do: reallocate the small lines
{
    int i;
    int old_width;
    int add_size = 0;

    i = 0;
    while(i < cub->map->height)
    {
        old_width = ft_strlen(cub->map->grid[i]);
        add_size = cub->map->width - old_width + 1;
        cub->map->grid[i] = ft_realloc(cub->map->grid[i], old_width, (old_width+add_size));
        while(old_width < cub->map->width)
        {           
            cub->map->grid[i][old_width] = ' ';
            old_width++;
        }
        i++;
    }
}

void    allocate_map(t_cub* cub)
{   
    cub->map = ft_calloc(1, sizeof(t_map));
    if (!cub->map)
        error_exit(cub, "Error\nMalloc failed for map struct\n");

    cub->map->height = 0;
    cub->map->width = 0;
    cub->map->grid = malloc(sizeof(char*) * (HEIGHT+1));
    if (!cub->map->grid)
        error_exit(cub, "Error\nMalloc failed for map grid\n");
}

int parse_file(char* filename, t_cub* cub)
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
        error_exit(cub, "Error\nError opening file\n");
    check_col_state(cub);
    allocate_map(cub);
    while(1)
    {
        line = get_next_line(fd);
        if (!line) 
            break;
        
        if(header_cnt < 6)
        {
            if(*line == '\n')
            {
                free(line);
                continue;
            }
            parse_text_col_line(line, &header_cnt, cub);
        }
        else 
        {
            if (header_cnt == 6) //meaning already 6 texture/colors
            {
                check_missing_text_col(cub);
                header_cnt++;       
            }
            if (!map_started && *line == '\n')
            {   
                free(line);
                continue;                
            }
            else
            {               
                if(*line == '\n')// here i should check also if previous line is just wall
                    error_exit(cub, "Error\nInvalid blank line inside map\n");

                trimd = ft_strtrim(line, "\r\n");
                validate_chars_in_map_line(trimd, &has_player, &map_started, cub);
                cub->map->grid[cub->map->height] = ft_strdup(trimd);
                
                free(trimd);
            }       
        }  
        
        free(line);        
    }

    //ft_printf("%s\n", cub->map->grid[1]);
    pad_map(cub);
    close(fd);
    if(!check_paths_accessibility(cub))
        error_exit(cub, "Error\nError opening texture file\n");
    return(1);
}

/*
✅ What I do need to check
Check	Where to do it	How
1️⃣ The identifier is valid (NO, SO, WE, EA)	Parser	your validate_identifier()
2️⃣ The second part exists (not empty)	Parser	if (*path == '\0') error_exit("Missing texture path");
3️⃣ The file exists and is accessible	After parsing	open(path, O_RDONLY)
4️⃣ The file ends with .xpm (mandatory format)

No need to close(fd) when error_exit() because on exit() kernel does clean it up. 
*/
