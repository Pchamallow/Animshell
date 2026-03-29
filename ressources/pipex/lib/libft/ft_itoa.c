/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 10:54:13 by pswirgie          #+#    #+#             */
/*   Updated: 2026/03/12 14:59:38 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_len(int n)
{
	int	i;

	i = 0;
	if (n < 0)
	{
		n = n * -1;
		i++;
	}
	while (n / 10 > 0)
	{
		n = n / 10;
		i++;
	}
	return (i + 1);
}

void	*ft_imalloc(int n)
{
	char	*str;

	str = (char *)malloc(sizeof(char) * (ft_len(n) + 1));
	if (!str)
		return (NULL);
	return (str);
}

char	*ft_diz(char *str, int n, int i)
{
	int	is_negative;

	is_negative = 0;
	if (n < 0)
	{
		n = n * -1;
		str[0] = '-';
		is_negative = 1;
	}
	while (i-- > is_negative)
	{
		str[i] = (n % 10) + '0';
		n = n / 10;
	}
	return (str);
}

char	*ft_itoa(int n)
{
	int		i;
	char	*str;

	i = ft_len(n);
	if (n == 0)
	{
		str = ft_strdup("0");
		return (str);
	}
	if (n == -2147483648)
	{
		str = ft_strdup("-2147483648");
		return (str);
	}
	str = ft_imalloc(n);
	if (str == NULL)
		return (NULL);
	ft_diz(str, n, i);
	str[i] = '\0';
	return (str);
}
