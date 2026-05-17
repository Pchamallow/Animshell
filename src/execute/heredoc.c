/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:07:23 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/16 16:15:18 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
while line != delimiter
- is a pipe = new line and print content to pipe
- no pipe = new line
*/
int	heredoc_lines(t_minishell *minishell, t_token *token, int fd)
{
	char	*line;
	int		signal;

	signal = 0;
	set_signal_heredoc();
	while (1)
	{
		/*printf("quotes = %d\n", token->quote);*/
		line = readline("> ");
		if (!line)
		{
			signal = check_signal_heredoc(token->value, signal);
			break;
		}
		/*if (!ft_strcmp(line, token->value))*/
			/*break;*/
		/*}*/
		// si quotes pas de expand !!!!!
		/*if (ft_strchr(line, '"') == NULL*/
			/*&& ft_strchr(line, '\'') == NULL*/
			/*&& token->quote == NO)*/
		if (token->quote != SINGLE)
			line = expand_line(line, minishell->exec.envp, minishell);
		if (!ft_strcmp(line, token->value))
		{
			free(line);
			break;
		}
		ft_printf_fd(fd, "%s\n", line);
		free(line);
	}
	return (signal);
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
	int		return_value;

	close_fd(&minishell->here_doc->fd);
	minishell->here_doc->fd = -1;
	
	pipe(pipefd);
	pid = fork();
	if (pid == 0)
	{
		close_fd(&fd);
		return_value = heredoc_lines(minishell, token, pipefd[1]);
		close_fd(&pipefd[0]);
		close_fd(&pipefd[1]);
		free_all(minishell);
		exit (return_value);
	}
	else
	{
		close_fd(&minishell->here_doc->fd);
		minishell->here_doc->fd = pipefd[0];
		close_fd(&pipefd[1]);
	}
	waitpid(pid, &return_value, 0);
	if (WIFEXITED(return_value))
		minishell->exec.error = WEXITSTATUS(return_value);
	return (0);
}

