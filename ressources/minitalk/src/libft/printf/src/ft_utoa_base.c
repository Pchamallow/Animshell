/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 17:15:56 by stkloutz          #+#    #+#             */
/*   Updated: 2025/11/22 17:17:26 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static int	ft_count(unsigned long nb)
{
	int	count;

	count = 1;
	while (nb >= 16)
	{
		nb = nb / 16;
		count++;
	}
	return (count);
}

char	*ft_utoa_base(unsigned long nb)
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
		str[count - 1] = HEXA[nb % 16];
		count--;
		nb = nb / 16;
	}
	return (str);
}
