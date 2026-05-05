/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 14:37:53 by stkloutz          #+#    #+#             */
/*   Updated: 2026/04/30 21:59:13 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig_value = 0;

void	signal_handler(int signal)
{
	g_sig_value = signal;
	if (signal == SIGINT)
	{
		rl_replace_line("",0);
		rl_on_new_line();
		rl_redisplay();
		write(1, "\nminishell$ ", 12);
	}
}

void	check_signal_value(t_minishell *minishell)
{
	if (g_sig_value == SIGINT)
		minishell->exec.error = 130;
	g_sig_value = 0;
}

void	set_signal_interactive(void)
{
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;

	//on initialise toute la struct à 0:
	ft_bzero(&sa_sigint, sizeof(struct sigaction));
	ft_bzero(&sa_sigquit, sizeof(struct sigaction));
	//fonction a executer en cas de reception d'un signal:
	sa_sigint.sa_handler = &signal_handler;
	sa_sigquit.sa_handler = SIG_IGN;
	//flags (utile ?) :
	sa_sigint.sa_flags = SA_RESTART;
	//sigaction avec les signaux a intercepter:
	sigaction(SIGINT, &sa_sigint, NULL);
	sigaction(SIGQUIT, &sa_sigquit, NULL);
}

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

void	get_exit_status(t_minishell *minishell)
{
	int	child_exit_status;
	int	exit_signal;

	while(wait(&child_exit_status) > 0)
	{
		if (WIFEXITED(child_exit_status))
			minishell->exec.error = WEXITSTATUS(child_exit_status);
		else if (WIFSIGNALED(child_exit_status))
		{
			exit_signal = WTERMSIG(child_exit_status);
			if (exit_signal == SIGINT)
			{
				minishell->exec.error = 130;
				write(1, "\n", 1);
			}
			else if (exit_signal == SIGQUIT)
			{
				minishell->exec.error = 131;
				write(1, "Quit", 4);
				if (WCOREDUMP(child_exit_status))
					write(1, " (core dumped)", 14);
				write(1, "\n", 1);
			}
		}
	}
}
