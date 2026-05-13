/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_strv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 11:29:03 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/13 11:35:11 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	memcpy_strv(char **dst, char **src, int max)
{
	int	i;

	i = 0;
	if (!src || !*src)
		return (1);
	while ((src[i] && max == 0) || (i < max))
	{
		dst[i] = src[i];
		i++;
	}
	return (0);
}

// int	memcpy_strv(char ***dst, char **src, int max)
// {
// 	int	i;

// 	i = 0;
// 	if (!*dst || !**dst || !src || !*src)
// 		return (1);
// 	while ((src[i] && max == 0) || (i < max))
// 	{
// 		(*dst)[i] = src[i];
// 		i++;
// 	}
// 	return (0);
// }