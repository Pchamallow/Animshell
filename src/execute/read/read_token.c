/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 16:07:17 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/14 11:40:41 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int next_pipe(t_minishell *minishell, t_token *token)
{
	t_token *tmp;
	int		i;
	int		is_pipe;

	i = 0;
	is_pipe = 0;
	tmp = token;
	// printf("%s\n", tmp->value);
	// printf("-------------------start search next pipe\n");
	// printf("old last_pipe index = %d\n", lastpipe);

	while (tmp)
	{
		// printf("%s\n", tmp->value);
		if (i > 0 && tmp->type == PIPE)
		{
			is_pipe = 1;
			if (tmp->next)
				tmp = tmp->next;
			break;
		}
		tmp = tmp->next;
		i++;
	}
	if (tmp)
	{
		if (is_pipe)
		{
			// printf("new last_pipe value -> %s\n", tmp->value);
			minishell->exec.last_pipe = tmp;
			return (i);
			// minishell->exec.index_pipe = minishell->exec.index_pipe + i;
		}
		else
			minishell->exec.index_pipe = -1;
	}
	return (0);
	// printf("-------------------end search next pipe\n");
}

int nb_args(t_token *token)
{
	t_token *tmp;
	int args;

	args = 0;
	tmp = token;
	if (tmp->type == PIPE)
		tmp = tmp->next;
	while (tmp != NULL && tmp->type != PIPE)
	{
		if (tmp->type == IS_ARG)
			args++;
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
static int init_cmd(t_minishell *minishell, t_pipe *pipe)
{
	t_token *token;
	int		nb_cmd_args;
	int		i;

	i = minishell->exec.index_prev_pipe;
	token = minishell->exec.last_pipe;
	nb_cmd_args = nb_args(token);
	while (token && (minishell->exec.index_pipe == 0 || i <= minishell->exec.index_pipe))
	{
		if (token->type == PIPE)
		{
			token = token->next;
			i++;
		}
		else if (token->type == IS_CMD)
		{
			if (path_cmd(minishell, token) == 0)
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
				pipe->input = ERROR;
				pipe->output = ERROR;
				return (1);
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
	if (pipe->is_cmd && nb_cmd_args > 0)
		init_cmd_args(minishell, pipe, nb_cmd_args);
	if (!pipe->is_cmd)
	{
		if (pipe->input == IS_HEREDOC)
		{
			close_fd(minishell->here_doc->fd);
			minishell->here_doc->fd = -1;
		}
		pipe->input = ERROR;
		pipe->output = ERROR;
	}
	return (0);
}

bool is_redirection(t_token *token)
{
	if (ft_strchr(token->value, '<') == NULL || ft_strchr(token->value, '>') == NULL)
		return (true);
	return (false);
}

void cpy_no_bzero(char *dst, const char *src, size_t size)
{
	size_t i;

	i = 0;
	while (i < (size - 1) && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
}

void convert_to_single_quotes(t_minishell *minishell, t_token *token)
{
	t_token *tmp_modify;
	char *original;
	int len;

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

void	remove_quots(t_minishell *minishell, t_token *token)
{
	char	*original;
	int		len;

	original = ft_strdup(token->value);
	if (!original)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	len = ft_strlen(original);
	free(token->value);
	token->value = ft_calloc(len, sizeof(char));
	ft_strlcpy(token->value, &original[1], len - 1);
	free(original);
	// printf("resultat = %s\n", token->value);
}

bool is_single_double_quoted(t_minishell *minishell, t_token *token)
{
	char	*str;
	int		i;
	int		single;
	int		doubled;

	i = 0;
	single = 0;
	doubled = 0;
	str = token->value;
	if (token->quote == SINGLE && is_double_quoted(token->value))
		return (false);
	if (str[0] == '\0')
		return (false);
	if (str[i] == '\'')
		single++;
	if (str[i] == '"')
		doubled++;
	i++;
	while (str[i])
	{
		if (str[i] == '\'')
			single = i;
		if (str[i] == '"')
			doubled = i;
		i++;
	}
	if (single > 1 && str[i] == '\'')
		remove_quots(minishell, token);
	if (doubled > 1 && str[i] == '\"')
		remove_quots(minishell, token);
	return (false);
}


/*
if a token is an infile or outfile and is double quoted
-> replace double quotes to simple quotes
else if token is an argument and we have a command
-> for '"'word'"' = one arg
-> keep token intact if it s single quoted + double quoted
-> add to char **cmd_args
*/
void read_args(t_minishell *minishell, t_token *token, t_pipe *pipe)
{
	int index_pipes;
	int i;

	i = minishell->exec.index_prev_pipe;
	index_pipes = minishell->exec.index_pipe;
	// gestion index pipes mauvaises
	while (token != NULL && ((index_pipes == 0) || (index_pipes > 0 && i <= index_pipes)))
	{
		// printf("token = %s\n", token->value);//test
		if (token->type == IS_ARG)
		{
			is_single_double_quoted(minishell, token);
			if (pipe->is_cmd == 1)
				add_args(minishell, pipe, token);
		}
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
int read_tokens(t_minishell *minishell, t_pipe *pipe)
{
	t_token *token;
	int		index_next_pipe;

	token = minishell->exec.last_pipe;
	if (find_input_output(minishell, pipe) || init_cmd(minishell, pipe))
	{
		minishell->exec.index_prev_pipe = minishell->exec.index_pipe;
		index_next_pipe = next_pipe(minishell, token);
		minishell->exec.index_pipe = minishell->exec.index_pipe + index_next_pipe;
		return (-1);
	}
	read_args(minishell, token, pipe);
	minishell->exec.index_prev_pipe = minishell->exec.index_pipe;
	index_next_pipe = next_pipe(minishell, token);
	minishell->exec.index_pipe = minishell->exec.index_pipe + index_next_pipe;

	return (0);
}
