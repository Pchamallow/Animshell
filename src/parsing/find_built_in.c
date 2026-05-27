/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 12:00:42 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/27 12:31:01 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**init_array_builtins(void)
{
	char	**builtins;

	builtins = malloc(sizeof(char *) * 8);
	if (!builtins)
		return (NULL);
	builtins[0] = ft_strdup("echo");
	builtins[1] = ft_strdup("cd");
	builtins[2] = ft_strdup("pwd");
	builtins[3] = ft_strdup("export");
	builtins[4] = ft_strdup("unset");
	builtins[5] = ft_strdup("env");
	builtins[6] = ft_strdup("exit");
	builtins[7] = NULL;
	if (!is_safe_strvlen(builtins, 7))
	{
		free_strv_len(builtins, 7);
		return (NULL);
	}
	return (builtins);
}

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

bool	find_built_in(t_minishell *minishell, char *token)
{
	char	**builtins;
	int		len_token;

	len_token = ft_strlen(token);
	builtins = init_array_builtins();
	if (!builtins)
		error_free_parsing(minishell);
	if (is_builtin(builtins, token, len_token))
	{
		free_strv(builtins);
		return (true);
	}
	free_strv(builtins);
	return (false);
}
