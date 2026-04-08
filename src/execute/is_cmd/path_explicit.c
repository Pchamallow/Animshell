/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_explicit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 15:11:20 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/08 21:29:22 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	path_explicit(t_parse *minishell, t_element *type)
// {
// 	int	len;

// 	len = ft_strlen(type->value);
// 	type->cmd_path = ft_calloc(sizeof(char *), len + 1);
// 	if (!type->cmd_path)
// 		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
// 	ft_strlcpy(type->cmd_path, type->value, len + 1);
// 	cmd_explicit(minishell, type);
// }

/*
if its a explicit path in token->value, 
"./value" -> "value",
for execve we need only the cmd
*/
void	cmd_explicit(t_minishell *minishell, t_token *token)
{
	int		len;

	len = ft_strlen(token->cmd_path) - 2;
	free(token->value);
	token->value = ft_calloc(sizeof(char *), len + 1);
	if (!token->value)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	ft_strlcpy(token->value, &token->cmd_path[2], len + 1);
}

char	*is_path(t_minishell *minishell, char **envp)
{
	int	i;
	int	is_path;

	i = 0;
	is_path = 0;
	while (envp[i])
	{
		if (ft_strnstr((const char *)envp[i], "PATH=", 5) != NULL)
		{
			is_path = 1;
			break ;
		}
		i++;
	}
	if (is_path == 0)
		print_error_free(minishell, "PATH not found.\n", 2);
	return (envp[i]);
}
