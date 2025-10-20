/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <mdiederi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 20:20:50 by mdiederi          #+#    #+#             */
/*   Updated: 2024/12/07 18:27:04 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_putunsignednbr_fd(unsigned int n, int *counter, int fd)
{
	char	single_dig;

	if (n <= 9)
	{
		single_dig = (n + '0');
		write(fd, &single_dig, 1);
		*counter += 1;
	}
	else if (n > 9)
	{
		ft_putunsignednbr_fd(n / 10, counter, fd);
		ft_putunsignednbr_fd(n % 10, counter, fd);
	}
	return ;
}

static void	ft_print_ps_arg(int *n_printed, const char c, void *arg)
{
	char	*str;

	if (c == 'p')
	{
		if (arg)
		{
			ft_putstr_fd("0x", 1);
			*n_printed = *n_printed + ft_strlen("0x");
			ft_print_hex(n_printed, (unsigned long)arg, "0123456789abcdef", 1);
		}
		else
		{
			ft_putstr_fd("(nil)", 1);
			*n_printed = *n_printed + ft_strlen("(nil)");
		}
	}
	else if (c == 's')
	{
		if (!arg)
			str = "(null)";
		else
			str = (char *)arg;
		ft_putstr_fd(str, 1);
		*n_printed = *n_printed + ft_strlen(str);
	}
}

static void	ft_print_cdi_arg(int *n_printed, const char c, int arg)
{
	if (c == 'c')
	{
		write(1, &arg, 1);
		*n_printed = *n_printed + 1;
	}
	if (c == 'd' || c == 'i')
	{
		ft_putnbr_fd(arg, 1);
		*n_printed = *n_printed + ft_num_len(arg);
	}
}

static void	ft_print_uxx_arg(int *n_printed, const char c, unsigned int arg)
{
	if (c == 'u')
		ft_putunsignednbr_fd(arg, n_printed, 1);
	else if (c == 'x')
		ft_print_hex(n_printed, (unsigned long)arg, "0123456789abcdef", 0);
	else if (c == 'X')
		ft_print_hex(n_printed, (unsigned long)arg, "0123456789ABCDEF", 0);
	return ;
}

int	ft_printf(const char *str, ...)
{
	va_list	valst;
	int		n_printed;

	n_printed = 0;
	if (!ft_check_str(str))
		return (n_printed);
	va_start(valst, str);
	while (*str)
	{
		if (*str == '%' && ft_is_in_set(*++str, "cspdiuxX"))
		{
			if (*str == 'p' || *str == 's')
				ft_print_ps_arg(&n_printed, *str, va_arg(valst, void *));
			if (*str == 'c' || *str == 'd' || *str == 'i')
				ft_print_cdi_arg(&n_printed, *str, va_arg(valst, int));
			if (*str == 'u' || *str == 'x' || *str == 'X')
				ft_print_uxx_arg(&n_printed, *str, va_arg(valst, unsigned int));
		}
		else
			ft_putchar_upcntr(&n_printed, *str);
		str++;
	}
	va_end(valst);
	return (n_printed);
}
