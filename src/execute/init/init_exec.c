/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 14:47:49 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/14 11:39:09 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_first_pipe(t_token *token)
{
	t_token *tmp;
	int pipe;
	int i;

	i = 0;
	pipe = 0;
	tmp = token;
	// printf("-------------------start find pipe actuel\n");
	while (tmp != NULL && tmp->type != PIPE)
	{
		// printf("%s\n", tmp->value);
		tmp = tmp->next;
		i++;
	}
	if (tmp != NULL && tmp->type == PIPE)
	{
		// printf("%s\n", tmp->value);
		pipe = 1;
	}
	// ft_printf_fd(2, "pipe index := %d\n", i);
	// ft_printf_fd(2, "last pipe:= %d\n", lastpipe);
	// printf("-------------------end find pipe actuel\n");
	return (pipe ? i : 0);
}

static void	init_heredoc(t_minishell *minishell)
{
	minishell->here_doc = malloc(sizeof(t_token));
	if (!minishell->here_doc)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	// minishell->here_doc->path_explicite = "/tmp/minishell_heredoc";
	// minishell->here_doc->path_explicite = "minishell_heredoc";
	minishell->here_doc->value = NULL;
	minishell->here_doc->fd = -1;
	// minishell->here_doc->path_explicite = ft_strdup("/tmp/minishell_heredoc");
	// minishell->here_doc->value = ft_strdup("minishell_heredoc");
}

/* 
Find nb of pipes, 
fill 0 or NULL,
*/
// void init_line_to_exec(t_minishell *minishell)
// {	
// 	init_pipe(minishell);
// }

/* Put values 0 or NULL in order to reuse after */
void	init_exec(t_minishell *minishell)
{
	t_token *tmp;
	int		first_pipe;

	first_pipe = find_first_pipe(minishell->token);
	if (first_pipe)
		minishell->exec.index_pipe = first_pipe;
	else
		minishell->exec.index_pipe = 0;
		// minishell->exec.index_pipe = lst_size(minishell->token);
	// printf("index first pipe = %d\n", first_pipe);
	// printf("nouvel index de la pipe = %d\n", minishell->exec.index_pipe);
	
	minishell->exec.input = 0;
	minishell->exec.output = 0;
	minishell->exec.index_prev_pipe = 0;
	minishell->exec.last_pipe = minishell->token;
	// minishell->exec.paths_for_search_cmd = NULL;
	// minishell->exec.envp = NULL;
	minishell->exec.pipe_lst = NULL;
	tmp = minishell->token;
	while (tmp != NULL)
	{
		tmp->args_execve = NULL;
		tmp->cmd_path = NULL;
		tmp->cmd_args = NULL;
		tmp->file_input = 0;
		tmp->file_output = 0;
		tmp = tmp->next;
	}
	minishell->builtin.echo.result = NULL;
	minishell->builtin.cd.result = NULL;
	init_heredoc(minishell);
}
