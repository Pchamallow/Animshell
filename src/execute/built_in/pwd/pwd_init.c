/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 16:22:29 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/27 16:22:47 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd_init(t_minishell *minishell)
{
	char	*str;

	str = getcwd(NULL, 0);
	if (!str)
	{
		minishell->builtin.cd.error = 1;
		return ;
	}
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
