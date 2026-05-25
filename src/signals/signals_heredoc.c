/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 21:12:40 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/25 10:39:09 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler_heredoc(int signal)
{
	g_sig_value = signal;
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		close(STDIN_FILENO);
	}
}

int	check_signal_heredoc(char *str, int signal)
{
	if (g_sig_value == SIGINT)
	{
		g_sig_value = 0;
		return (130);
	}
	else
	{
		ft_printf_fd(2, "minishell: warning: ");
		ft_printf_fd(2, "here-document delimited by end-of-file ");
		ft_printf_fd(2, "(wanted '%s')\n", str);
		return (signal);
	}
}

void	get_exit_heredoc(t_minishell *minishell, int return_value, int pid)
{
	waitpid(pid, &return_value, 0);
	if (WIFEXITED(return_value))
		minishell->exec.error = WEXITSTATUS(return_value);
}

void	set_signal_heredoc(void)
{
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;

	ft_bzero(&sa_sigint, sizeof(struct sigaction));
	ft_bzero(&sa_sigquit, sizeof(struct sigaction));
	sa_sigint.sa_handler = &signal_handler_heredoc;
	sa_sigquit.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_sigint, NULL);
	sigaction(SIGQUIT, &sa_sigquit, NULL);
}
