/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 16:07:17 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/06 15:20:21 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*is_path(t_minishell *minishell)
{
	char	*paths;

	paths = getenv("PATH");
	// ft_printf_fd(2, "%s\n", paths);
	if (paths == NULL)
		print_error_free(minishell, "PATH not found.\n", 2);
	return (paths);
}

/*
1. is a PATH ?
2. is a explicit path, absolute path ?
3. is cmd valid ?
type 0 = WORD
type 4 = IS_CMD
*/
int is_cmd(t_minishell *minishell)
{
	char	**all_paths;
	char	*paths_one_line;
	t_token	*token;

	paths_one_line = is_path(minishell);
	all_paths = ft_split(paths_one_line, ':');
	token = &minishell->token;
	while(token != NULL)
	{
		if (token->type == 0 || token->type == 4)
			path_cmd(minishell, token, all_paths);
		token = token->next;
	}
	return (0);
}
