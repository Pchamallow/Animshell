/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 16:47:25 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/13 12:06:02 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**init_new_envp(t_minishell *minishell, int len)
{
	char	**new_envp;
	new_envp = malloc(sizeof(char *) * (len + 3));
	if (!new_envp)
	{
		free(minishell->builtin.pwd.result);
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
	new_envp[0] = ft_calloc
		(ft_strlen(minishell->builtin.pwd.result) + 1, sizeof(char));
	if (!new_envp[0])
	{
		free(minishell->builtin.pwd.result);
		free(new_envp);
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
	return (new_envp);
}

void	add_pwd_to_envp(t_minishell *minishell)
{
	int		len;
	char	**new_envp;

	len = get_envp_len(minishell->exec.envp);
	new_envp = init_new_envp(minishell, len);
	ft_strlcpy(new_envp[0], minishell->builtin.pwd.result,
		ft_strlen(minishell->builtin.pwd.result) + 1);
	memcpy_strv(&new_envp[1], minishell->exec.envp, len);
	new_envp[len + 1] = NULL;
	free(minishell->exec.envp);
	minishell->exec.envp = new_envp;
}

void	init_pwd(t_minishell *minishell)
{
	char	*str;

	str = getcwd(NULL, 0);
	if (minishell->builtin.pwd.result)
		free(minishell->builtin.pwd.result);
	minishell->builtin.pwd.result = ft_strjoin("PWD=", str);
	if (!minishell->builtin.pwd.result)
	{
		free(str);
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
	free(str);
}

void	init_pwd_envp(t_minishell *minishell)
{
	init_pwd(minishell);
	if (strv_searchindex(minishell->exec.envp, "PWD=") == -1)
		add_pwd_to_envp(minishell);
}

int	pwd(t_minishell *minishell, t_pipe *pipe)
{
	(void)pipe;
	init_pwd(minishell);
	if (minishell->builtin.pwd.result)
		printf("%s\n", &minishell->builtin.pwd.result[4]);
	return (0);
}
