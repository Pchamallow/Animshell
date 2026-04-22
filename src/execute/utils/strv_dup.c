/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strv_dup.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:29:46 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/22 18:12:58 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int	strv_dup(t_minishell *minishell, char ***dst, char **src)
{
	int	len;
	int	i;

	i = 0;
	if (!src)
		return(-1);
	len = len_double(src) + 2;
	*dst = (char **)malloc((sizeof(char *)) * len);
	if (!*dst)
		print_error_free(minishell, "Error\nMalloc failed.\n", 1);
	len += 1;
	while (src[i])
	{
		(*dst)[i] = ft_strdup(src[i]);
		if (!(*dst)[i])
			print_error_free(minishell, "Error\nMalloc failed.\n", 1);;
		i++;
	}
	(*dst)[i] = NULL;
	return (0);
}
