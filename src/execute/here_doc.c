/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:07:23 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/27 18:23:55 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_permissions(t_minishell *minishell, t_pipe *pipe, int *fd)
{
	int *fd;

	fd = &minishell->here_doc->fd;
	if (*fd < 0)
	{
		pipe->output = ERROR;
		printf("error out\n");
		if (pipe->input != ERROR)
			strerror_file(token->value);
		minishell->exec.error = 2;
	}
	if (access(token->value, W_OK | R_OK) != 0)
	{
		// printf("error out\n");
		// strerror_file(token->value);
		pipe->output = ERROR;
		minishell->exec.error = 1;
		return (-1);
	}
}

/*
comportement de here doc 

delimiter =
ecire du texte, 
tant qu il n y a pas une ligne avec seulement
le delimiter, on continue

ce texte est le infile de la commande 

comme our les autres infiles, seul le dernier delimiter est 
pris en compte comme infile

delimiter 
1. declencher le readline avec un > en prompt
2. avoir un while on a pas seulement le delimiter
3. mettre le contenu des lignes dans un fichier ? 



*/

void    heredoc(t_minishell *minishell, t_pipe *pipe, t_token *token)
{
	int *fd;

	fd = &minishell->here_doc->fd;
	*fd = open("/tmp/minishell_heredoc", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (heredoc_permissions(minishell, pipe))
		return (0);

}