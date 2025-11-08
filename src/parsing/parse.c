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

void    populate_color(char* trimd, char *ident, t_cub* cub)
{
    char* color;

    color = second_part(ident, trimd, cub);   
    if(validate_color(color, cub))
    {
        if(*ident == 'F')
            cub->col->floor = int_color(color);
        else if(*ident == 'C')
            cub->col->ceil = int_color(color);        
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

int check_valid_ident(char *trimd, char *ident)
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

void populate_address(char* trimd, char *ident, t_cub* cub)
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
            cub->col->no_tex_p = token;
        else if(ft_strncmp(ident, "SO", 2) == 0)
            cub->col->so_tex_p = token;
        else if(ft_strncmp(ident, "WE", 2) == 0)
            cub->col->we_tex_p = token;
        else if(ft_strncmp(ident, "EA", 2) == 0)
            cub->col->ea_tex_p = token;
    }
    
}

void    parse_text_col_line(char *line, t_cub* cub)
{
    char *trimd = ft_strtrim(line, " \n\t\f\v\r");
    if(!trimd)
        error_exit(cub, "Error\nMalloc failed!\n");
    if(*trimd == '\0')
        error_exit(cub, "Error\nEmpty line\n");

    if(check_valid_ident(trimd, "NO"))
        populate_address(trimd, "NO", cub);
    else if(check_valid_ident(trimd, "SO"))
        populate_address(trimd, "SO", cub);
    else if(check_valid_ident(trimd, "WE"))   
        populate_address(trimd, "WE", cub);
    else if(check_valid_ident(trimd, "EA"))
        populate_address(trimd, "EA", cub);
    else if(check_valid_ident(trimd, "F"))
        populate_color(trimd, "F", cub);        
    else if(check_valid_ident(trimd, "C"))
        populate_color(trimd, "C", cub);
    else
        error_exit(cub, "Error\nInvalid Identifier\n");
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

int parse_file(char* filename, t_cub* cub)
{
    int fd;
    char *line;
    
    fd = open(filename, O_RDONLY);
    if (fd == -1)
        error_exit(cub, "Error\nError opening file\n");

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
    while(1)
    {
        line = get_next_line(fd);
        if( line == NULL)
            break;
        parse_text_col_line(line, cub);
        
        free(line);
    }
    close(fd);
    if (!cub->col->no_tex_p || !cub->col->so_tex_p ||
        !cub->col->we_tex_p || !cub->col->ea_tex_p)
        error_exit(cub, "Error\nMissing texture path(s)\n");

    if(!check_paths_accessibility(cub))
        error_exit(cub, "Error\nError opening texture file\n");
   
    return(1);
}

/*
✅ What I do need to check
Check	Where to do it	How
1️⃣ The identifier is valid (NO, SO, WE, EA)	Parser	your check_valid_ident()
2️⃣ The second part exists (not empty)	Parser	if (*path == '\0') error_exit("Missing texture path");
3️⃣ The file exists and is accessible	After parsing	open(path, O_RDONLY)
4️⃣ The file ends with .xpm (mandatory format)
*/
