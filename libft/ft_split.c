/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <mdiederi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:05:19 by mdiederi          #+#    #+#             */
/*   Updated: 2024/11/17 12:18:48 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_is_valid_input(char const *str, char c)
{
	char	**ar;

	if (!str || str[0] == '\0')
	{
		ar = (char **)malloc(sizeof(char *));
		if (ar)
			ar[0] = NULL;
		return (ar);
	}
	while (*str)
	{
		if (*str != c)
			return (NULL);
		str++;
	}
	ar = (char **)malloc(sizeof(char *));
	if (ar)
		ar[0] = NULL;
	return (ar);
}

static size_t	ft_get_ndelims(char const *str, char c)
{
	size_t	ndelims;

	ndelims = 0;
	if (!str)
		return (ndelims);
	while (*str == c)
		str++;
	while (*str)
	{
		if (*str == c)
		{
			ndelims++;
			while (*str == c)
				str++;
		}
		else
			str++;
	}
	if (ndelims > 0 && *(str - 1) == c)
		ndelims--;
	return (ndelims);
}

static char	*ft_set_next_substr(char const *s, char c, size_t *str_ind)
{
	char	*substr;
	size_t	len_substr;
	size_t	start;

	start = 0;
	start += *str_ind;
	while (s[start] == c)
		start++;
	len_substr = 0;
	while (s[start + len_substr] && s[start + len_substr] != c)
		len_substr++;
	*str_ind = len_substr + start;
	if (len_substr > 0)
	{
		substr = malloc(len_substr + 1);
		if (substr == NULL)
			return (NULL);
		ft_strlcpy(substr, &s[start], len_substr + 1);
		return (substr);
	}
	return (NULL);
}

static void	ft_free_array(char **ar, size_t i)
{
	while (i > 0)
		free(ar[--i]);
	free(ar);
}

char	**ft_split(char const *s, char c)
{
	char	**ar;
	size_t	i;
	size_t	str_ind;

	ar = ft_is_valid_input(s, c);
	if (ar)
		return (ar);
	ar = (char **)malloc((ft_get_ndelims((char *)s, c) + 2) * sizeof(char *));
	if (ar == NULL)
		return (NULL);
	str_ind = 0;
	i = 0;
	while (i < (ft_get_ndelims((char *)s, c) + 1))
	{
		ar[i] = ft_set_next_substr(s, c, &str_ind);
		if (ar[i] == NULL)
		{
			ft_free_array(ar, i);
			return (NULL);
		}
		i++;
	}
	ar[i] = NULL;
	return (ar);
}
