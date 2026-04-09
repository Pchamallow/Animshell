/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 16:07:17 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/09 20:38:10 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cpy(char **dst, char *src)
{
	if (*dst != NULL)
		free(*dst);
	*dst = ft_strdup(src);
}

/*
TODO verifier que ca fait bien toute la pipe
*/
int	find_pipe(t_token *token, int lastpipe)
{
	t_token *tmp;
	int	pipe;
	int	i;
	int	skip;

	pipe = 0;
	i = 0;
	skip = 0;
	tmp = token;
	while (tmp != NULL && tmp->type != PIPE)
	{
		tmp = tmp->next;
		if (i >= lastpipe)
			i++;
		skip++;
	}
	if (tmp->type == PIPE)
	{
		pipe = 1;
		if (i > 0)
			i--;
	}
	// ft_printf_fd(2, "pipe : %d\n", i);
	return (pipe ? i : 0);
}

void	next_pipe(t_minishell *minishell, t_token *token, int lastpipe)
{
	t_token *tmp;
	int		i;

	i = 0;
	tmp = token;
	while (tmp != NULL && i <= lastpipe)
	{
		tmp = tmp->next;
		i++;	
	}
	while (tmp != NULL && tmp->type != PIPE)
		tmp = tmp->next;
	if (tmp != NULL && tmp->type == PIPE)
		minishell->exec.last_pipe = tmp;
}

int	nb_args(t_token *token)
{
	t_token *tmp;
	int		args;

	args = 0;
	tmp = token;
	if (tmp->type == PIPE)
		tmp = tmp->next;
	while (tmp != NULL && tmp->type != PIPE)
	{
		if (tmp->type == IS_ARG)
		{
			args++;
			// ft_printf_fd(2, "args == %s\n", tmp->value);
		}
		tmp = tmp->next;
	}
	return (args);
}

static void	init_cmd(t_minishell *minishell, t_pipe *pipe,
	char **all_paths, int pipes)
{
	t_token *token;
	int	nb_cmd_args;
	int	i;

	i = 0;
	token = minishell->exec.last_pipe;
	nb_cmd_args = nb_args(token);
	while (token != NULL && (pipes == 0 || i <= pipes))
	{
		// ft_printf_fd(2, "read token : %s\n", token->value);
		// ft_printf_fd(2, "%d\n", nb_cmd_args);
		if (token->type == IS_CMD)
		{
			path_cmd(minishell, token, all_paths);
			pipe->cmd = token;
			pipe->is_cmd = 1;
			ft_printf_fd(2, "cmd pipe value : %s\n", pipe->cmd->value);
		}
		else if (token->type == IS_BUILT_IN)
		{
			is_built_in(minishell, token);	
			pipe->cmd = token;
			pipe->is_cmd = 1;
		}
		token = token->next;
		i++;
	}
	if (nb_cmd_args != 0)
		init_cmd_args(minishell, pipe, nb_cmd_args);

}

/*
A. IS_CMD
path_cmd : 
1. is an environnement variable PATH ?
2. is a explicit path, absolute path ? 
Or path is to search
3. is cmd valid ?

B. IS_BUILT_IN
1. what built in is ? 

- input_pipe = input is pipe in
*/

/*!SECTION
-> avoir le prochain pipe en token + en index

*/
int read_tokens(t_minishell *minishell, t_pipe *pipe, t_token *token, char **envp)
{
	char	**all_paths;
	char	*paths_one_line;
	int		i;
	int		index_pipes;
	int		input_pipe;

	// print_pauline(minishell);
	i = 0;
	token = minishell->exec.last_pipe;
	input_pipe = 0;
	if (minishell->exec.last_pipe->type == PIPE)
		input_pipe = 1;
	// ft_printf_fd(2, "ICI : %s\n", token->value);
	index_pipes = find_pipe(token, minishell->exec.index_pipe);
	minishell->exec.index_pipe = index_pipes;
	paths_one_line = is_path(minishell, envp);
	all_paths = ft_split(paths_one_line, ':');
	init_cmd(minishell, pipe, all_paths, index_pipes);
	next_pipe(minishell, token, index_pipes);
	while ((token != NULL && index_pipes == 0)
		|| (index_pipes > 0 && token != NULL && i <= index_pipes))
	{
		ft_printf_fd(2, "pipe = %s\n", token->value);
		// if (token->type == IS_BUILT_IN)
		// 	is_built_in(minishell, token);
		if (token->type == IS_ARG)
			add_args(minishell, pipe, token);
		else if (token->type == IS_FILENAME)
			read_files(minishell, pipe, token);
		else if (token->type == IS_INPUT && token->next != NULL)
			token->next->input = 1;
		else if (token->type == IS_OUTPUT && token->next != NULL)
			token->next->output = 1;
		token = token->next;
		i++;
	}
	if (!pipe->infile)
	{
		if (input_pipe == 1)
			pipe->input = IS_PIPE;
		else
			pipe->input = 0;
	}
	// il y a un dernier infile, mais il y a une erreure
	// donc une erreure a print apres echo
	// if (minishell->exec.input == -1)
	// ft_printf_fd(2, "ici\n");
	free_double(all_paths);
	return (0);
}
