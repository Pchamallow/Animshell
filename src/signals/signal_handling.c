/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 21:27:27 by stkloutz          #+#    #+#             */
/*   Updated: 2026/04/25 15:43:34 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig_value = 0;

void	signal_handler(int signal)
{
	if (signal == SIGINT)
		g_sig_value = 130;
	if (signal == SIGQUIT)
		g_sig_value = 131;
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
	sa_sigquit.sa_handler = SIG_IGN;//pour ignorer le signal
	//sigaction avec les signaux a intercepter:
	sigaction(SIGINT, &sa_sigint, NULL);
	sigaction(SIGQUIT, &sa_sigquit, NULL);
}

void	set_signal_default(void)
{
	struct sigaction sa_default;

	ft_bzero(&sa_default, sizeof(struct sigaction));
	sa_default.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa_default, NULL);
	sigaction(SIGQUIT, &sa_default, NULL);
}
