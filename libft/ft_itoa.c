/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <mdiederi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 19:42:39 by mdiederi          #+#    #+#             */
/*   Updated: 2024/11/12 13:13:46 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_ndigits_incl_sign(int n)
{
	size_t	n_dig;

	n_dig = 1;
	if (n == -2147483648)
		return (11);
	if (n < 0)
	{
		n = (-1) * n;
		n_dig += 1;
	}
	while ((n / 10) > 0)
	{
		n = n / 10;
		n_dig++;
	}
	return (n_dig);
}

char	*ft_itoa(int n)
{
	size_t	n_digits;
	char	*str;

	n_digits = ft_ndigits_incl_sign(n);
	str = malloc(n_digits + 1);
	if (str == NULL)
		return (NULL);
	if (n == -2147483648)
	{
		ft_strlcpy(str, "-2147483648", n_digits + 1);
		return (str);
	}
	str[n_digits--] = '\0';
	if (n < 0)
	{
		str[0] = '-';
		n = n * (-1);
	}
	while (n > 9 && n_digits > 0)
	{
		str[n_digits--] = (n % 10) + '0';
		n = n / 10;
	}
	str[n_digits] = n + '0';
	return (str);
}
