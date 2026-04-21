/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 16:07:17 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/21 15:59:07 by pswirgie         ###   ########.fr       */
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
	if (tmp != NULL && tmp->type == PIPE)
	{
		pipe = 1;
		// if (i > 0)
		// 	i--;
	}
	// ft_printf_fd(2, "pipe index := %d\n", i);
	// ft_printf_fd(2, "last pipe:= %d\n", lastpipe);
	return (pipe ? i : 0);
}

void	next_pipe(t_minishell *minishell, t_token *token, int lastpipe)
{
	t_token *tmp;
	int		i;

	i = 0;
	tmp = token;
	while (tmp != NULL && i < lastpipe)
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

/*
Init_cmd 

A. IS_CMD
path_cmd : 
1. is an environnement variable PATH ?
2. is a explicit path, absolute path ? 
Or path is to search
3. is cmd valid ?
	Path_cmd 
		1. is an environnement variable PATH ?
		2. is a explicit path, absolute path ? 
		Or path is to search
		3. is cmd valid ?

B. IS_BUILT_IN
1. what built in is ? 

C. if there is a command and at least one arg 
= initialisation tab args
*/
static int	init_cmd(t_minishell *minishell, t_pipe *pipe,
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
		if (token->type == IS_CMD)
		{
			if (path_cmd(minishell, token, all_paths) == 0)
			{
				pipe->cmd = token;
				pipe->is_cmd = 1;
			}
			else
			{
				token->type = WORD;
				if (pipe->input != ERROR && pipe->output != ERROR)
					ft_printf_fd(2, "minishell: %s: command not found\n", token->value);
				minishell->exec.error = 127;
				return (-1);
			}
		}
		else if (token->type == IS_BUILT_IN)
		{
			is_built_in(pipe, token);	
			pipe->cmd = token;
			pipe->is_cmd = 1;
		}
		token = token->next;
		i++;
	}
	if (pipe-> is_cmd == 1 && nb_cmd_args > 0)
		init_cmd_args(minishell, pipe, nb_cmd_args);
	return (0);
}

bool is_redirection(t_token *token)
{
	if (ft_strchr(token->value, '<') == NULL
		|| ft_strchr(token->value, '>') == NULL)
		return (true);
	return (false);
}

void	cpy_no_bzero(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	while (i < (size - 1) && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
}

void	convert_to_single_quotes(t_minishell *minishell, t_token *token)
{
	t_token *tmp_modify;
	char	*original;
	int		len;

	len = ft_strlen(token->value);
	tmp_modify = token;
	original = ft_calloc(len + 1, sizeof(char));
	if (!tmp_modify->value)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	ft_strlcpy(original, token->value, len);
	free(tmp_modify->value);
	tmp_modify->value = ft_calloc(len + 1, sizeof(char));
	if (!tmp_modify->value)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	tmp_modify->value[0] = '\'';
	cpy_no_bzero(tmp_modify->value, &original[1], len - 1);
	tmp_modify->value[len] = '\'';
	free(original);
}


/*
if a token is an infile or outfile and is double quoted
-> replace double quotes to simple quotes
else if token is an argument and we have a command
-> keep token intact if it s single quoted + double quoted
-> add to char **cmd_args
*/
void	read_args(t_minishell* minishell, t_token *token, t_pipe *pipe)
{
	int	index_pipes;
	int	i;

	i = 0;
	index_pipes = minishell->exec.index_pipe;
	while (token != NULL && ((index_pipes == 0)
		|| (index_pipes > 0 && i <= index_pipes)))
	{
		// ft_printf_fd(2, "pipe = %s\n", token->value);
		if ((token->quote == DOUBLE ||
			(token->quote == SINGLE && token->next && token->next->quote != DOUBLE))
			&& (is_redirection(token) == true))
			convert_to_single_quotes(minishell, token);
			// printf("ICI = %s\n", token->value);
		if (token->type == IS_ARG && pipe->is_cmd == 1)
			add_args(minishell, pipe, token);
		token = token->next;
		i++;
	}
}



/*
1. read files
- error files before command

2. init command

Pipe : 
-> input pipe : input from previous pipe
-> ouput pipe : output to next pipe

*/
int read_tokens(t_minishell *minishell, t_pipe *pipe, char **envp)
{
	char	**all_paths;
	char	*paths_one_line;
	int		index_pipes;
	int		error_files;
	int		error_cmd;
	// int		input_pipe;
	t_token	*token;

	token = minishell->exec.last_pipe;

	// printf ("READ TOKENS\n");
	
	// input_pipe = 0;
	// if (minishell->exec.last_pipe->type == PIPE)
	// 	input_pipe = 1;
	error_cmd = 0;
	error_files = 0;
	/* CMD et Infile et Outfile valides **************************/
	index_pipes = find_pipe(token, minishell->exec.index_pipe);
	minishell->exec.index_pipe = index_pipes;
	paths_one_line = is_path(minishell, envp);
	all_paths = ft_split(paths_one_line, ':');
	error_files = read_files(minishell, pipe, index_pipes);
	error_cmd = init_cmd(minishell, pipe, all_paths, index_pipes);
	if (!(error_files == 0 && error_cmd == 0))
	{
		next_pipe(minishell, token, index_pipes);
		// close_fds_pipe(minishell->exec.pipe_lst);
		free_double(all_paths);
		return (-1);
	}
	/**************************************************************/

	next_pipe(minishell, token, index_pipes);

	read_args(minishell, token, pipe);

	// if (!pipe->infile)
	// {
	// 	if (input_pipe == 1)
	// 		pipe->input = IS_PIPE;
	// 	// else
	// 		// pipe->input = 0;
	// }
	// il y a un dernier infile, mais il y a une erreure
	// donc une erreure a print apres echo
	// if (minishell->exec.input == -1)
	// ft_printf_fd(2, "ici\n");
	free_double(all_paths);
	return (0);
}
