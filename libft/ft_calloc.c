/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <mdiederi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 19:40:37 by mdiederi          #+#    #+#             */
/*   Updated: 2024/11/15 08:17:04 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ar;
	size_t	nbytes;

	nbytes = 0;
	if (size == 0 || nmemb == 0)
		nbytes = 0;
	else if (nmemb > 2147483647 / size)
		return (NULL);
	else
		nbytes = nmemb * size;
	ar = (char *)malloc(nbytes);
	if (ar == NULL)
		return (NULL);
	ft_bzero(ar, nbytes);
	return (ar);
}
