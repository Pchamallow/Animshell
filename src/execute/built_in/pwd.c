/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 16:47:25 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/07 13:37:14 by pswirgie         ###   ########.fr       */
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
	i = strv_searchindex(minishell->exec.envp, "PWD=");
	if (i != -1)
	{
		path = ft_strdup(&minishell->exec.envp[i][4]);
		ft_printf_fd(1, "%s\n", path);
		free(path);
	}
	else
		ft_printf_fd(2, "minishell: Can't find pwd.\n");
		//erreur si je ne trouve pas pwd ?
	return (0);
}
