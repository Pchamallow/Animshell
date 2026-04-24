/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:08:45 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/24 17:07:22 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_infile(t_minishell *minishell, t_pipe *pipe, t_token *token)
{
	token->fd = open(token->value, O_RDONLY);
	// close(token->fd);
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
		// pipe->infile = NULL;
		pipe->input = ERROR;
		minishell->exec.error = 1;
		return (-1);
	}
	return (0);
}

static int	init_outfile(t_minishell *minishell, t_pipe *pipe, t_token *token)
{
	token->fd = open(token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// printf("fd out = %d\n", token->fd);
	// printf("fd out = %s\n", token->value);
	if (token->fd < 0)
	{
		pipe->output = ERROR;
		// printf("error out\n");
		if (pipe->input != ERROR)
			strerror_file(token->value);
		minishell->exec.error = 2;
	}
	if (access(token->value, W_OK) != 0)
	{
		// printf("error out\n");
		// strerror_file(token->value);
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

	i = 0;
	/*garder les erreurs de tests */
	/* permission OK, exit ou est creer pour un outfile*/
	token = minishell->exec.last_pipe;
	while (token && i < minishell->exec.index_pipe)
	{
		if (token->type == IS_INPUT && token->next != NULL)
			token->next->file_input = 1;
		else if (token->type == IS_OUTPUT && token->next != NULL)
			token->next->file_output = 1;

		if (token->file_input == 1)
		{
			if ((init_infile(minishell, pipe, token) == 0))
			{
				pipe->infile = token;
				pipe->input = IS_FILE;
			}
			// else
			// 	return (-1);
		}
		
		if (pipe->output != IS_FILE && pipe->output != ERROR && token->type == PIPE)
			pipe->output = IS_PIPE;
		
		else if (token->file_output == 1)
		{
			if (init_outfile(minishell, pipe, token) == 0)
			{
				pipe->outfile = token;
				pipe->output = IS_FILE;
				// close(pipe->outfile->fd);
			}
			// else
			// 	return (-1);
		}
		
		token = token->next;
		i++;
	}
	if (pipe->input == TERMINAL && minishell->exec.nb_pipes)
	{
		pipe->input = IS_PIPE;
		minishell->exec.nb_pipes--;
	}
	if (pipe->input == ERROR || pipe->output == ERROR)
		return (-1);
	// if (pipe->next)
	// 	pipe->output = IS_PIPE;
	// 	pipe->write = 1;
	// else 
	// 	pipe->write = 0;

	// if (pipe->input == IS_PIPE)
	// 	pipe->read = 1;
	// else
	// 	pipe->read = 0;
		
	// printf("output of pipe == %d\n", pipe->output);
	return (0);
}
