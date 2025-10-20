/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <mdiederi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 19:43:09 by mdiederi          #+#    #+#             */
/*   Updated: 2024/11/12 13:26:23 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*area_1;
	const unsigned char	*area_2;
	size_t				i;

	if (n == 0)
		return (0);
	area_1 = (const unsigned char *)s1;
	area_2 = (const unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (area_1[i] > area_2[i])
			return (1);
		else if (area_1[i] < area_2[i])
			return (-1);
		i++;
	}
	return (0);
}
