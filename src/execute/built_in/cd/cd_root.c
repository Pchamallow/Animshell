/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_root.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 15:47:23 by pswirgie          #+#    #+#             */
/*   Updated: 2026/06/03 09:47:31 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
