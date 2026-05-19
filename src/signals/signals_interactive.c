/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_interactive.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 14:37:53 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/19 22:05:24 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig_value = 0;

void	signal_handler(int signal)
{
	g_sig_value = signal;
	if (signal == SIGINT)
	{
		rl_replace_line("", 0);
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

	ft_bzero(&sa_sigint, sizeof(struct sigaction));
	ft_bzero(&sa_sigquit, sizeof(struct sigaction));
	sa_sigint.sa_handler = &signal_handler;
	sa_sigquit.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_sigint, NULL);
	sigaction(SIGQUIT, &sa_sigquit, NULL);
}
