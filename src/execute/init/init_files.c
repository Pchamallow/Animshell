/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:08:45 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/30 16:21:39 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_infile(t_minishell *minishell, t_pipe *pipe, t_token *token)
{
	if (is_directory(minishell, pipe, token->value))
	{
		pipe->infile_error = 1;
		return (-1);
	}
	token->fd = open(token->value, O_RDONLY);
	if (token->fd < 0)
	{
		pipe->infile_error = 1;
		minishell->exec.error = 2;
		strerror_file(token->value);
	}
	if (access(token->value, R_OK) != 0)
	{
		pipe->infile_error = 1;
		minishell->exec.error = 1;
		return (-1);
	}
	return (0);
}

/*
init_outfile

- if file_output == 2 -> append
write in file after previous content

- else erase, write in file
*/
static int	init_outfile(t_minishell *minishell, t_pipe *pipe, t_token *token)
{
	if (is_directory(minishell, pipe, token->value))
	{
		pipe->outfile_error = 1;
		return (-1);
	}
	if (token->file_output == 2)
		token->fd = open(token->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		token->fd = open(token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (token->fd < 0)
	{
		pipe->outfile_error = 1;
		minishell->exec.error = 2;
		if (!pipe->infile_error)
			strerror_file(token->value);
	}
	if (access(token->value, W_OK) != 0)
	{
		pipe->outfile_error = 1;
		minishell->exec.error = 1;
		return (-1);
	}
	return (0);
}

void	set_input_output(t_minishell *minishell, t_pipe *pipe, t_token *token)
{
	if (token->type == IS_INPUT && token->next != NULL)
		token->next->file_input = 1;
	else if (token->type == IS_OUTPUT && token->next != NULL)
		token->next->file_output = 1;
	else if (token->type == IS_APPEND && token->next != NULL)
		token->next->file_output = 2;
	else if (token->file_input && !pipe->infile_error
		&& (init_infile(minishell, pipe, token) == 0))
	{
		if (pipe->input == IS_FILE)
			close_fd(&pipe->infile->fd);
		pipe->infile = token;
		pipe->input = IS_FILE;
	}
	else if (token->file_output && !pipe->outfile_error
		&& init_outfile(minishell, pipe, token) == 0)
	{
		if (pipe->output == IS_FILE)
			close_fd(&pipe->outfile->fd);
		pipe->outfile = token;
		pipe->output = IS_FILE;
	}
}

int	handle_heredoc(t_minishell *minishell, t_pipe *pipe, t_token *token, int fd)
{
	if (token->type == IS_DELIMITER)
	{
		if (pipe->input == IS_FILE)
			close_fd(&pipe->infile->fd);
		heredoc(minishell, token, fd);
		if (!pipe->infile_error)
			pipe->input = IS_HEREDOC;
		return (1);
	}
	return (0);
}

/*
find_input_output

- if a token is an input, next token is a file input
- if a token is an output, next token is a file ouput
- if we have a pipe, input = pipe, else if we have infile 
input = file
*/
int	find_input_output(t_minishell *minishell, t_pipe *pipe, int fd)
{
	t_token	*token;
	int		heredoc_pipe_to_free;

	heredoc_pipe_to_free = 0;
	if (minishell->exec.index_prev_pipe > 0)
		pipe->input = IS_PIPE;
	token = minishell->exec.last_pipe;
	pipe->is_cmd = is_cmd(token);
	while (token && !is_pipe(pipe, token))
	{
		set_input_output(minishell, pipe, token);
		heredoc_pipe_to_free += handle_heredoc(minishell, pipe, token, fd);
		if (minishell->exec.error == 130)
			break ;
		token = token->next;
	}
	pipe->is_cmd = 0;
	if (heredoc_pipe_to_free && pipe->input != IS_HEREDOC)
		close_fd(&minishell->here_doc->fd);
	if (pipe->infile_error || pipe->outfile_error)
		return (1);
	return (0);
}
