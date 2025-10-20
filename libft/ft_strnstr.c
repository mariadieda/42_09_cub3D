/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <mdiederi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:40:32 by mdiederi          #+#    #+#             */
/*   Updated: 2024/11/11 19:21:30 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	llen;

	i = 0;
	llen = ft_strlen(little);
	if (*little == '\0')
		return ((char *)big);
	while (*big && (i + llen <= len))
	{
		if (ft_strncmp(big, little, llen) == 0)
			return ((char *)big);
		big++;
		i++;
	}
	return (NULL);
}
