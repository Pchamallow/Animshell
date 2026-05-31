/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_default.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 21:14:00 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/31 21:07:20 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_signal_to_default(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(struct sigaction));
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	ignore_signal(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(struct sigaction));
	sa.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	get_signal_status(t_minishell *minishell, int child_exit_status)
{
	int		exit_signal;

	exit_signal = WTERMSIG(child_exit_status);
	if (exit_signal == SIGINT)
	{
		minishell->exec.error_sig = 130;
		write(1, "\n", 1);
	}
	else if (exit_signal == SIGQUIT)
	{
		minishell->exec.error_sig = 131;
		write(1, "Quit", 4);
		if (WCOREDUMP(child_exit_status))
			write(1, " (core dumped)", 14);
		write(1, "\n", 1);
	}
}

void	get_exit_status(t_minishell *minishell, pid_t last_pid)
{
	int		child_exit_status;
	pid_t	wpid;

	wpid = waitpid(-1, &child_exit_status, 0);
	while (wpid > 0)
	{
		if (wpid == last_pid)
		{
			if (WIFEXITED(child_exit_status))
				minishell->exec.error = WEXITSTATUS(child_exit_status);
		}
		if (WIFSIGNALED(child_exit_status))
			get_signal_status(minishell, child_exit_status);
		wpid = waitpid(-1, &child_exit_status, 0);
	}
}
