/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:07:23 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/29 16:42:45 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_lines(t_minishell *minishell, t_token *token, int fd)
{
	char	*line;
	int		signal;

	signal = 0;
	set_signal_heredoc();
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			signal = check_signal_heredoc(token->value, signal);
			break ;
		}
		if (token->quote != SINGLE)
			line = expand_line(line, minishell->exec.envp, minishell);
		if (!ft_strcmp(line, token->value))
		{
			free(line);
			break ;
		}
		ft_printf_fd(fd, "%s\n", line);
		free(line);
	}
	return (signal);
}

static int	heredoc_child(t_minishell *minishell, t_token *token, int *pipefd, int *fd)
{
	int	return_value;

	return_value = 0;
	close_fd(fd);
	return_value = heredoc_lines(minishell, token, pipefd[1]);
	close_fd(&pipefd[0]);
	close_fd(&pipefd[1]);
	free_all(minishell);
	exit (return_value);
	return (return_value);
}

/*
heredoc
- reads the input until a line containing the delimiter is seen
- this input is the infile of the command
- if several inputs, only the last one is used by the command
*/
int	heredoc(t_minishell *minishell, t_token *token, int fd)
{
	pid_t	pid;
	int		pipefd[2];
	int		return_value;
	int		prev_error;

	return_value = 0;
	close_fd(&minishell->here_doc->fd);
	minishell->here_doc->fd = -1;
	prev_error = minishell->exec.error;
	pipe(pipefd);
	pid = fork();
	if (pid == 0)
		return_value = heredoc_child(minishell, token, pipefd, &fd);
	else
	{
		minishell->here_doc->fd = pipefd[0];
		close_fd(&pipefd[1]);
	}
	get_exit_heredoc(minishell, return_value, pid);
	if (minishell->exec.error == 0)
		minishell->exec.error = prev_error;
	return (0);
}
