/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 16:47:25 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/05 18:54:15 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_pwd(t_minishell *minishell)
{
	int	i;

	i = 0;
	while (minishell->exec.envp[i])
	{
		if (ft_strnstr((const char *)minishell->exec.envp[i], "PWD=", 5) != NULL)
			return (i);
		i++;
	}
	return (-1);
}

int	pwd(t_minishell *minishell, t_pipe *pipe)
{
	char	*path;
	int		i;

	i = 0;
	(void)pipe;
	if (!minishell->exec.envp || !minishell->exec.envp[0])
		return (0);
	i = find_pwd(minishell);
	if (i != -1)
	{
		path = ft_strdup(&minishell->exec.envp[i][4]);
		ft_printf_fd(1, "%s\n", path);
		free(path);
	}
	else
		ft_printf_fd(2, "cant find\n");
		//erreur si je ne trouve pas pwd ?
	return (0);
}
