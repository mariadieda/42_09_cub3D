/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <mdiederi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:06:30 by mdiederi          #+#    #+#             */
/*   Updated: 2024/11/12 13:27:00 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	len;
	size_t	dst_len;
	size_t	src_len;

	len = 0;
	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (dstsize <= dst_len)
		return (dstsize + src_len);
	else
	{
		while ((src[len] != '\0') && (len < (dstsize - dst_len - 1)))
		{
			dst[dst_len + len] = src[len];
			len++;
		}
		dst[dst_len + len] = '\0';
	}
	return (dst_len + src_len);
}
