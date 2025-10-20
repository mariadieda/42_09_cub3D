/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <mdiederi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 19:42:54 by mdiederi          #+#    #+#             */
/*   Updated: 2024/11/12 13:17:01 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*address_s;

	i = 0;
	address_s = (unsigned char *)s;
	while (i < n)
	{
		if (address_s[i] == (unsigned char)c)
			return (&address_s[i]);
		i++;
	}
	return (NULL);
}
