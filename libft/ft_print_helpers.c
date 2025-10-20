/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <mdiederi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 17:40:10 by mdiederi          #+#    #+#             */
/*   Updated: 2024/12/07 18:15:04 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putchar_upcntr(int *n_printed, const char c)
{
	write(1, &c, 1);
	*n_printed = *n_printed + 1;
}

int	ft_is_in_set(char c, char const *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

int	ft_check_str(const char *str)
{
	if (!str)
	{
		write(1, "", 1);
		return (0);
	}
	else
		return (1);
}

int	ft_num_len(int num)
{
	int		len;
	char	*numstr;

	len = 0;
	numstr = ft_itoa(num);
	len = ft_strlen(numstr);
	free(numstr);
	return (len);
}

void	ft_print_hex(int *n_printed, unsigned long arg, char *hex_vals,
		int is_long)
{
	unsigned long	next;

	if (!is_long)
		arg = (unsigned int)arg;
	if (arg >= 16)
	{
		next = arg / 16;
		ft_print_hex(n_printed, next, hex_vals, 1);
	}
	else if ((int)arg < 0)
	{
		ft_putchar_fd('0', 1);
		*n_printed += 1;
		return ;
	}
	ft_putchar_fd((hex_vals[arg % 16]), 1);
	*n_printed += 1;
}
