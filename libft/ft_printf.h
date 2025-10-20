/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiederi <mdiederi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 14:40:42 by mdiederi          #+#    #+#             */
/*   Updated: 2024/12/11 17:44:10 by mdiederi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft.h"
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

int		ft_printf(const char *str, ...);
int		ft_num_len(int num);
int		ft_check_str(const char *str);
int		ft_is_in_set(char c, char const *set);
void	ft_print_hex(int *n_printed, unsigned long arg, char *hex_vals,
			int is_long);
void	ft_putchar_upcntr(int *n_printed, const char c);

#endif /* FT_PRINTF_H */