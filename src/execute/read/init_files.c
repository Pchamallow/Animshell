/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:08:45 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/10 12:39:56 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_infile(t_minishell *minishell, t_pipe *pipe, t_token *token)
{
	token->fd = open(token->value, O_RDONLY);
	if (token->fd < 0)
	{
		pipe->input = -1;
		token->close = 1;
		minishell->exec.error = 2;
		strerror_print(token->value); //NOTE -  print ici ou stocker pour print aprs echo [par exemple] ? 
	}
	if (access(token->value, R_OK) != 0)
	{
		free(minishell->exec.file_input);
		pipe->input = -1;
		minishell->exec.error = 1;
		return (-1);
	}
	return (0);
}

static int	init_outfile(t_minishell *minishell, t_pipe *pipe, t_token *token)
{
	token->fd = open(token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (token->fd < 0)
	{
		pipe->output = ERROR;
		token->close = 1;
		strerror_print(token->value);
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

void	read_files(t_minishell *minishell, t_pipe *pipe, t_token *token)
{
	/*garder les erreurs de tests */
	/* permission OK, exit ou est creer pour un outfile*/
	if (token->file_input == 1 && (init_infile(minishell, pipe, token) == 0))
	{
		pipe->infile = token;
		pipe->input = IS_FILE;
			// free_cpy(&pipe->infile->value, token->value);
	}
	if (token->file_output == 1 && (init_outfile(minishell, pipe, token) == 0))
	{
		// printf("\nICI\n");
		pipe->outfile = token;
		pipe->output = IS_FILE;
			// free_cpy(&pipe->outfile->value, token->value);
	}
}
