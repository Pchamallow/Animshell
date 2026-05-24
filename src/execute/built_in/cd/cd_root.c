/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_root.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 15:47:23 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/24 15:58:37 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	root_from_pwd(char *pwd)
{
	int	slash;
	int	i;

	i = 0;
	slash = 0;
	while (pwd[i] && slash < 3)
	{
		if (pwd[i] == '/')
			slash++;
		i++;
	}
	return (i);
}

int	is_root(t_minishell *minishell)
{
	int	result;
	int	len;

	result = cpy_strvindex(&minishell->builtin.cd.result,
			minishell->exec.envp, "HOME=");
	if (result == 1)
		return (1);
	else if (result == -1)
	{
		len = root_from_pwd(minishell->builtin.pwd.result);
		minishell->builtin.cd.result = ft_substr(minishell->builtin.pwd.result,
				4, len - 4);
		if (!minishell->builtin.cd.result)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
	return (0);
}

void	root_with_folder(t_minishell *minishell)
{
	char	*home;
	char	*folder;
	int		result;

	result = cpy_strvindex(&home, minishell->exec.envp, "HOME=");
	if (result == 1)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	else if (result == -1)
		return ;
	folder = ft_substr(minishell->builtin.cd.result, 1,
			ft_strlen(minishell->builtin.cd.result) - 1);
	if (!folder)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	free(minishell->builtin.cd.result);
	minishell->builtin.cd.result = ft_strjoin(home, folder);
	free(folder);
	free(home);
}
