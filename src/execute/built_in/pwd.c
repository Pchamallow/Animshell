/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 16:47:25 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/05 17:06:16 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(t_minishell *minishell, t_pipe *pipe)
{
	char	*path;
	int		i;

	i = 0;
	(void)pipe;
	if (!minishell->exec.envp || !minishell->exec.envp[0])
		return (0);
	while (minishell->exec.envp[i])
	{
		if (ft_strnstr((const char *)minishell->exec.envp[i], "PWD=", 5) != NULL)
		{
			path = ft_strdup(&minishell->exec.envp[i][4]);
			ft_printf_fd(1, "%s\n", path);
			free(path);
			return (1);
		}
		i++;
	}
	return (0);
}
