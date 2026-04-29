/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:07:23 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/29 10:05:03 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
while line != delimiter
- is a pipe = new line and print content to pipe
- no pipe = new line
*/
void	heredoc_lines(t_minishell *minishell)
{
	char	*line;
	bool	read;
	bool	is_pipe;

	read = true;
	// is_pipe = false;
	is_pipe = true;
	while (read == true)
	{
		if (is_pipe == true)
		{
			// si quotes pas de expand !!!!!
			line = readline("> ");
			if (!ft_strcmp(line, minishell->here_doc->value))
				break;
			line = expand_line(line, minishell->exec.envp);
			ft_printf_fd(1, "%s\n", line);
			free(line);
		}
		else
		{
			line = readline("> ");
			if (!ft_strcmp(line, minishell->here_doc->value))
				break;
			free(line);
		}
	}
	free(minishell->here_doc->value);
	minishell->here_doc->value = NULL;
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


- in delimiter, if we have a quote (double or single)
not expand
*/
// CTRL + D = error
int	heredoc(t_minishell *minishell, t_pipe *pipe, t_token *token)
{
	(void)pipe;
	
	minishell->here_doc->value = ft_strdup(token->value);

	heredoc_lines(minishell);

	return (0);
	
}
