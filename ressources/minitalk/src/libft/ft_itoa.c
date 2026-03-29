/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:14:06 by stkloutz          #+#    #+#             */
/*   Updated: 2025/10/25 16:56:35 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(unsigned int nb, int n)
{
	int	count;

	count = 1;
	while (nb >= 10)
	{
		nb = nb / 10;
		count++;
	}
	if (n < 0)
		count++;
	return (count);
}

char	*ft_itoa(int n)
{
	char			*str;
	unsigned int	nb;
	int				count;

	str = NULL;
	if (n < 0)
		nb = n * -1;
	else
		nb = n;
	count = ft_count(nb, n);
	str = ft_calloc(count + 1, sizeof(char));
	if (!str)
		return (NULL);
	while (count > 0)
	{
		str[count - 1] = (nb % 10) + '0';
		count--;
		nb = nb / 10;
		if (count == 0 && n < 0)
		{
			str[count] = '-';
			count--;
		}
	}
	return (str);
}
