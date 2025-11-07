#include "cub3d.h"


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

char*    second_part(char *ident, char* trimd, t_cub* cub)
{
    size_t len_ident;
    const char * temp;
    char * s;
    //if(!ident || !trimd) //they can't be empty so I removed the check
        //return (NULL);

    len_ident = ft_strlen(ident);
   
    temp = trimd + len_ident; //find the end of the identifier and make it start of the temp
    temp = skip_ws(temp); // if there are white spaces at the beginning remove them 
   
    if(*temp == '\0')
    {
        free(trimd);
        error_exit("Error\nEmpty address!\n", cub);
    }
        
    s = ft_strdup(temp); // give back a malloced string    
    if(!s)
    {
        free(trimd);
        error_exit("Error\nMalloc failed!\n", cub);
    }
    
    return (s);
}

t_rgb int_color(char *ident, char* line, t_cub* cub) //to fix...
{
    t_rgb colors;
    char **nums;

    char* s1 = second_part(ident, line, cub);
    nums = ft_split(second_part, ',');
    colors.r = ft_atoi(nums[0]);
    colors.g = ft_atoi(nums[1]);
    colors.b = ft_atoi(nums[2]);
    free_split(); //to do
    return (colors);
}


void    parse_line(char *line, t_cub* cub)
{    
    char *trimd = ft_strtrim(line, " \n\t\f\v\r");
    if(!trimd)
        error_exit("Error\nMalloc failed!\n", cub);
    if(*trimd == '\0')
        error_exit("Error\nEmpty line\n", cub);

    if(check_valid_ident(trimd, "NO"))
        cub->col->no_tex_p = second_part("NO", trimd, cub);
    else if(ft_strnstr(line, "SO", 3))
        cub->col->so_tex_p = second_part("SO", line, cub);
    else if(ft_strnstr(line, "WE", 3))   
        cub->col->we_tex_p = second_part("WE", line, cub);
    else if(ft_strnstr(line, "EA", 3))
        cub->col->ea_tex_p = second_part("EA", line, cub);
    else if(ft_strnstr(line, "F", 3))
        cub->col->floor = int_color("F", line, cub);
    else if(ft_strnstr(line, "C", 3))
        cub->col->ceil = int_color("C", line, cub);
    else
        error_exit("Error\nInvalid Identifier\n", cub);
    free(trimd);
}

int parse_file(char* filename, t_cub* cub)
{
    int fd;
    char *line;
    
    fd = open(filename, O_RDONLY);
    if (fd == -1)
        error_exit(cub, "Error\nError opening file\n");
    while(1)
    {
        line = get_next_line(fd);
        if( line == NULL)
            break;
        parse_line(line, cub); 
        free(line);    
    }
    close(fd);
}

/*
✅ What I do need to check
Check	Where to do it	How
1️⃣ The identifier is valid (NO, SO, WE, EA)	Parser	your check_valid_ident()
2️⃣ The second part exists (not empty)	Parser	if (*path == '\0') error_exit("Missing texture path");
3️⃣ The file exists and is accessible	After parsing	open(path, O_RDONLY)
4️⃣ The file ends with .xpm (mandatory format)
*/
