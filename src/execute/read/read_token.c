/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 16:07:17 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/09 14:50:52 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cpy(char *dst, char *src)
{
	if (dst != NULL)
		free(dst);
	dst = ft_strdup(src);
}

void	read_files(t_minishell *minishell, t_token *token)
{
	/*garder les erreurs de tests */
	/* permission OK, exit ou est creer pour un outfile*/
	if (token->input == 1)
	{
		if (init_infile(minishell, token) == 0)
			free_cpy(minishell->exec.file_input, token->value);
	}
	if (token->output == 1)
		free_cpy(minishell->exec.file_output, token->value);
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

static void	init_cmd(t_minishell *minishell, t_token **pipe,
	char **all_paths, int pipes)
{
	t_token *token;
	int	nb_cmd_args;
	int	i;

	i = 0;
	token = minishell->exec.last_pipe;
	nb_cmd_args = nb_args(token);
	ft_printf_fd(2, "ICI = %d\n", nb_cmd_args);
	while ((token != NULL && pipes == 0)
			|| (pipes > 0 && token != NULL && i <= pipes))
	{
		// ft_printf_fd(2, "read token : %s\n", token->value);
		// ft_printf_fd(2, "%d\n", i);
		if (token->type == IS_CMD)
		{
			path_cmd(minishell, token, all_paths);
			*pipe = token;
			// ft_printf_fd(2, "cmd pipe value : %s\n", (*pipe)->value);
		}
		else if (token->type == IS_BUILT_IN)
		{
			is_built_in(minishell, token);	
			*pipe = token;
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

*/

/*!SECTION
-> avoir le prochain pipe en token + en index

*/
int read_tokens(t_minishell *minishell, t_token **pipe,
	t_token *token, char **envp)
{
	char	**all_paths;
	char	*paths_one_line;
	int		i;
	int		index_pipes;

	// print_pauline(minishell);
	i = 0;
	token = minishell->exec.last_pipe;
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
		if (token->type == IS_BUILT_IN)
			is_built_in(minishell, token);
		else if (token->type == IS_ARG)
			add_args(minishell, pipe, token);
		else if (token->type == IS_FILENAME)
			read_files(minishell, token);
		else if (token->type == IS_INPUT && token->next != NULL)
			token->next->input = 1;
		else if (token->type == IS_OUTPUT && token->next != NULL)
			token->next->output = 1;
		token = token->next;
		i++;
	}
	// il y a un dernier infile, mais il y a une erreure
	// donc une erreure a print apres echo
	// if (minishell->exec.input == -1)
	// ft_printf_fd(2, "ici\n");
	free_double(all_paths);
	return (0);
}
