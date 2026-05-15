/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:07:23 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/15 13:10:44 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
while line != delimiter
- is a pipe = new line and print content to pipe
- no pipe = new line
*/
void	heredoc_lines(t_minishell *minishell, t_token *token, int fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!ft_strcmp(line, token->value))
			break;
		// si quotes pas de expand !!!!!
		if (ft_strchr(line, '"') == NULL
			&& ft_strchr(line, '\'') == NULL
			&& token->quote == NO)
			line = expand_line(line, minishell->exec.envp, minishell);
		ft_printf_fd(fd, "%s\n", line);
		free(line);
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


- in delimiter, if we have a quote (double or single)
not expand
*/
// CTRL + D = error
int	heredoc(t_minishell *minishell, t_token *token, int fd)
{
	pid_t	pid;
	int		pipefd[2];

	close_fd(&minishell->here_doc->fd);
	minishell->here_doc->fd = -1;
	
	pipe(pipefd);
	pid = fork();
	if (pid == 0)
	{
		close_fd(&fd);
		heredoc_lines(minishell, token, pipefd[1]);
		close_fd(&pipefd[0]);
		close_fd(&pipefd[1]);
		free_all(minishell);
		exit (0);
	}
	else
	{
		close_fd(&minishell->here_doc->fd);
		minishell->here_doc->fd = pipefd[0];
		close_fd(&pipefd[1]);
	}
	waitpid(pid, NULL, 0);
	// while(wait(NULL) > 0);
	return (0);
}
