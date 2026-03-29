/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 17:12:49 by stkloutz          #+#    #+#             */
/*   Updated: 2026/02/15 23:09:39 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "../libft.h"
# include <stdarg.h>
# define VAR_LIST "cspdiuxX%"
# define HEXA "0123456789abcdef"

int		ft_printf(const char *format, ...);
char	*ft_utoa(unsigned long nb);
char	*ft_utoa_base(unsigned long nb);
char	*ft_str_toupper(char *str);
void	ft_write_null(int *count, char c);
void	ft_var_c(char c, int *count);
void	ft_var_s(char *s, int *count);
void	ft_var_dori(int d, int *count);
void	ft_var_u(unsigned int u, int *count);
void	ft_var_p(unsigned long p, int *count);
void	ft_var_x(unsigned int u, int *count);
void	ft_var_upperx(unsigned int u, int *count);

#endif
