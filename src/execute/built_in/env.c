/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 10:31:25 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/02 22:44:41 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	****************************************************************
** ENV
**	env with no options or arguments:
**		- returns an error if options or arguments are found
**		- lists all environnement variables, except the hidden ones
**	****************************************************************
*/
int	env(t_minishell *minishell, t_pipe *pipe)
{
	int	i;

	minishell->exec.error = 0;
	if (pipe->nb_args > 0)
	{
		ft_printf_fd(2, "Error: too many arguments\n");
		minishell->exec.error = 2;
		return (minishell->exec.error);
	}
	i = 0;
	while (minishell->exec.envp[i])
	{
		if (ft_strchr(minishell->exec.envp[i], '='))
			ft_printf_fd(1, "%s\n", minishell->exec.envp[i]);
		i++;
	}
	return (minishell->exec.error);
}
