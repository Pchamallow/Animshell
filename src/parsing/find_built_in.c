/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 12:00:42 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/27 12:38:30 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_builtin(char **builtins, char *token, int len_token)
{
	int	len_builtin;
	int	i;

	i = 0;
	while (i < 7)
	{
		len_builtin = ft_strlen(builtins[i]);
		if (len_token > len_builtin)
		{
			if (ft_strncmp(token, builtins[i], len_token) == 0)
				return (1);
		}
		else
		{
			if (ft_strncmp(token, builtins[i], len_builtin) == 0)
				return (1);
		}
		i++;
	}
	return (0);
}

bool	find_built_in(char *token)
{
	char	*builtins[7];
	int		len_token;

	len_token = ft_strlen(token);
	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	if (is_builtin(builtins, token, len_token))
		return (true);
	return (false);
}
