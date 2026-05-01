/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 10:31:25 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/01 12:51:40 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	------------------------------------------------------------	*/
/*	env with no options or arguments as requested by the subject	*/
/*	lists all environnement variables, except the hidden ones		*/
/*	------------------------------------------------------------	*/
int	env(t_minishell *minishell, t_pipe *pipe)
{
	int	i;

	if (pipe->nb_args > 0)
	{
		ft_printf_fd(2, "Error: too many arguments\n");
		return (2);
	}
	i = 0;
	while (minishell->exec.envp[i])
	{
		ft_printf_fd(1, "%s\n", minishell->exec.envp[i]);
		i++;
	}
	return (0);
}
