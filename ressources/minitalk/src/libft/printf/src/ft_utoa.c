/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 16:36:26 by stkloutz          #+#    #+#             */
/*   Updated: 2025/11/22 17:14:29 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static int	ft_count(unsigned long nb)
{
	int	count;

	count = 1;
	while (nb >= 10)
	{
		nb = nb / 10;
		count++;
	}
	return (count);
}

char	*ft_utoa(unsigned long nb)
{
	char			*str;
	int				count;

	str = NULL;
	count = ft_count(nb);
	str = ft_calloc(count + 1, sizeof(char));
	if (!str)
		return (NULL);
	while (count > 0)
	{
		str[count - 1] = (nb % 10) + '0';
		count--;
		nb = nb / 10;
	}
	return (str);
}
