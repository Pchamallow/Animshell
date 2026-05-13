/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:08:45 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/13 16:11:32 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_infile(t_minishell *minishell, t_pipe *pipe, t_token *token)
{
	token->fd = open(token->value, O_RDONLY);
	if (token->fd < 0)
	{
		pipe->input = ERROR;
		minishell->exec.error = 2;
		strerror_file(token->value); //NOTE -  print ici ou stocker pour print aprs echo [par exemple] ? 
	}
	if (access(token->value, R_OK) != 0)
	// F_OK pour qu il existe, a verifier
	// X_OK executable 
	{
		pipe->input = ERROR;
		minishell->exec.error = 1;
		return (-1);
	}
	return (0);
}

/*
init_outfile

- if file_output == 2 -> append
write in file after previous contente

- else erase, write in file
*/
static int	init_outfile(t_minishell *minishell, t_pipe *pipe, t_token *token)
{
	if (token->file_output == 2)
		token->fd = open(token->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		token->fd = open(token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (token->fd < 0)
	{
		pipe->output = ERROR;
		if (pipe->input != ERROR)
			strerror_file(token->value);
		minishell->exec.error = 2;
	}
	if (access(token->value, W_OK) != 0)
	{
		pipe->output = ERROR;
		minishell->exec.error = 1;
		return (-1);
	}
	return (0);
}

/*
read_files

- if a token in an input, next token is a file input
- if a token in an output, next token is a file ouput
- if we have a pipe, input = pipe, else if we have infile 
input = file
*/
int	find_input_output(t_minishell *minishell, t_pipe *pipe)
{
	t_token *token;
	int		i;
	int		heredoc_pipe_to_free;

	i = minishell->exec.index_prev_pipe;
	heredoc_pipe_to_free = 0;
	if (i > 0)
		pipe->input = IS_PIPE;
	token = minishell->exec.last_pipe;
	while (token && i <= minishell->exec.index_pipe)
	{
		if (token->type == IS_INPUT && token->next != NULL)
			token->next->file_input = 1;
		else if (token->type == IS_OUTPUT && token->next != NULL)
			token->next->file_output = 1;
		else if (token->type == IS_APPEND && token->next != NULL)
			token->next->file_output = 2;
		else if (token->file_input
			&& (init_infile(minishell, pipe, token) == 0))
		{
			if (pipe->input == IS_FILE)
				close_fd(pipe->infile->fd);
			pipe->infile = token;
			pipe->input = IS_FILE;
		}
		else if (i > minishell->exec.index_prev_pipe
			&& pipe->output != IS_FILE
			&& pipe->output != ERROR && token->type == PIPE)
			pipe->output = IS_PIPE;
		
		else if (token->file_output
			&& init_outfile(minishell, pipe, token) == 0
			&& pipe->output != ERROR)
		{
			if (pipe->output == IS_FILE)
				close_fd(pipe->outfile->fd);
			pipe->outfile = token;
			pipe->output = IS_FILE;
		}
		else if (token->type == IS_DELIMITER)
		{
			if (pipe->input == IS_FILE)
				close_fd(pipe->infile->fd);
			heredoc(minishell, token);
			heredoc_pipe_to_free = 1;
			if (pipe->input != ERROR)
				pipe->input = IS_HEREDOC;
		}
		
		token = token->next;
		i++;
	}
	if (heredoc_pipe_to_free && pipe->input != IS_HEREDOC)
		close_fd(minishell->here_doc->fd);
	if (pipe->input == ERROR || pipe->output == ERROR)
		return (1);
	return (0);
}
