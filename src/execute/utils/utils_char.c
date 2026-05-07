/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 11:11:40 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/07 13:26:32 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*str_beginend_char(t_minishell *minishell, char *str, char c)
{
	char	*new;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str) + 3;
	new = ft_calloc(len, sizeof(char));
	if (!new)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	new[0] = c;
	ft_strlcpy(&new[1], str, len);
	new[len - 2] = c;
	return (new);
}

int	index_lastchar(char *str, char c)
{
	int	i;

	if (!str)
		return (0);
	i = ft_strlen(str) - 1;
	while (i >= 0 && str[i] && str[i] != c)
		i--;
	return (i);
}

int	is_sign(char c)
{
	if (c == '<' || c == '>' || c == '|' || c == '&' || c == '-')
		return (1);
	return (0);
}

int	join_oldnew(char **old, char **new)
{
	char	*new_path;

	new_path = ft_strjoin(*old, *new);
	if (!new_path)
		return (1);
	free(*new);
	*new = new_path;
	free(*old);
	return (0);
}
