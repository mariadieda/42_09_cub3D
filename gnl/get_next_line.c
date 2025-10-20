/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <mdiederi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 21:07:16 by mdiederi          #+#    #+#             */
/*   Updated: 2025/01/11 14:58:40 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*fill_buffer(int fd, ssize_t *nbytesread, char *new_buffer,
		char *stat_str)
{
	*nbytesread = read(fd, new_buffer, BUFFER_SIZE);
	if (*nbytesread == -1)
	{
		free(stat_str);
		free(new_buffer);
		return (NULL);
	}
	new_buffer[*nbytesread] = '\0';
	return (new_buffer);
}

static char	*generate_str_with_eol(int fd, char *stat_str)
{
	char	*new_buffer;
	char	*tmp;
	ssize_t	nbytesread;

	if (!stat_str)
	{
		stat_str = malloc(1);
		stat_str[0] = '\0';
	}
	new_buffer = malloc(BUFFER_SIZE + 1);
	if (!new_buffer)
		return (NULL);
	nbytesread = 1;
	while (nbytesread > 0 && !ft_strchr(stat_str, '\n'))
	{
		new_buffer = fill_buffer(fd, &nbytesread, new_buffer, stat_str);
		if (!new_buffer)
			return (NULL);
		tmp = ft_strjoin(stat_str, new_buffer);
		free(stat_str);
		stat_str = tmp;
	}
	free(new_buffer);
	return (stat_str);
}

static char	*extract_line(char *str, int *eol_pos)
{
	char	*next_line;
	int		index;

	index = 0;
	while (str[index] != '\0' && str[index] != '\n')
		index++;
	if (str[index] == '\n')
		index++;
	*eol_pos = index;
	next_line = malloc(index + 1);
	index = -1;
	while (++index < *eol_pos)
		next_line[index] = str[index];
	next_line[index] = '\0';
	return (next_line);
}

char	*rm_line(char *strleft, int eol_pos)
{
	int		j;
	char	*new_strleft;
	int		size_rest;

	if (!strleft)
		return (NULL);
	size_rest = ft_strlen(strleft) - eol_pos;
	if (size_rest == 0)
	{
		free(strleft);
		strleft = NULL;
		return (NULL);
	}
	new_strleft = malloc((size_rest + 1));
	j = 0;
	while (strleft[eol_pos])
		new_strleft[j++] = strleft[eol_pos++];
	new_strleft[j] = '\0';
	free(strleft);
	strleft = NULL;
	return (new_strleft);
}

char	*get_next_line(int fd)
{
	static char	*strleft;
	char		*line;
	int			eol_pos;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	eol_pos = 0;
	strleft = generate_str_with_eol(fd, strleft);
	if (!strleft)
	{
		free(strleft);
		return (NULL);
	}
	line = extract_line(strleft, &eol_pos);
	strleft = rm_line(strleft, eol_pos);
	if (line && line[0] == '\0')
	{
		free(line);
		free(strleft);
		strleft = NULL;
		return (NULL);
	}
	return (line);
}
