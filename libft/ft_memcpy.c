/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <mdiederi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:03:50 by mdiederi          #+#    #+#             */
/*   Updated: 2024/11/12 13:20:03 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*dest_address;
	unsigned char	*src_address;

	dest_address = (unsigned char *)dest;
	src_address = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		dest_address[i] = src_address[i];
		i++;
	}
	return (dest);
}
