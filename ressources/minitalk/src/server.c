/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 22:02:20 by stkloutz          #+#    #+#             */
/*   Updated: 2026/03/07 09:19:04 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minitalk.h"

volatile sig_atomic_t	g_sig_value = 0;

void	handle_sigterm(int sig)
{
	if (sig == SIGINT)
	{
		g_sig_value = -1;
		ft_putendl_fd("\nSIGINT received", 1);
	}
	if (sig == SIGQUIT)
	{
		g_sig_value = -1;
		ft_putendl_fd("\nSIGQUIT received", 1);
	}
}

void	handle_sigusr_server(int sig, siginfo_t *info, void *context)
{
	(void)context;
	if (sig == SIGUSR1)
		g_sig_value = 1;
	if (sig == SIGUSR2)
		g_sig_value = 2;
	if (info->si_pid > 0)
		kill(info->si_pid, SIGUSR1);
}

void	update_str(t_chr *current, t_list **msg, t_str *str)
{
	if (current->c != '\0')
	{
		((char *)str->tmp->content)[str->i] = current->c;
		str->i++;
		if (str->i == LEN_LIMIT - 1)
		{
			add_msg(msg);
			str->tmp = str->tmp->next;
			str->i = 0;
		}
	}
	else
	{
		print_str(*msg);
		ft_lstclear(msg, free);
		*msg = NULL;
		add_msg(msg);
		str->tmp = *msg;
		ft_bzero(str->tmp->content, LEN_LIMIT);
		str->i = 0;
	}
	ft_bzero(current, sizeof(*current));
}

void	get_msg(t_chr *current, t_list *msg, t_str *str)
{
	while (1)
	{
		if (g_sig_value == -1)
		{
			ft_lstclear(&msg, free);
			exit (0);
		}
		if (g_sig_value > 0)
		{
			current->c <<= 1;
			if (g_sig_value == 2)
				current->c |= 1;
			current->bit_count++;
			if (current->bit_count == 8)
				update_str(current, &msg, str);
			g_sig_value = 0;
		}
		pause();
	}
}

int	main(void)
{
	t_sigaction	sa;
	t_chr		current;
	t_list		*msg;
	t_str		str;

	ft_printf("SERVER PID : %d\n", getpid());
	ft_bzero(&sa, sizeof(sa));
	ft_bzero(&current, sizeof(current));
	msg = NULL;
	add_msg(&msg);
	str.i = 0;
	str.tmp = msg;
	init_sigaction(&sa);
	if (sigaction(SIGUSR1, &sa.sigusr, NULL) == -1
		|| sigaction(SIGUSR2, &sa.sigusr, NULL) == -1
		|| sigaction(SIGINT, &sa.sigterm, NULL) == -1
		|| sigaction(SIGQUIT, &sa.sigterm, NULL) == -1)
	{
		ft_lstclear(&msg, free);
		ft_putendl_fd(ERR_SIG, 2);
		return (1);
	}
	get_msg(&current, msg, &str);
	return (0);
}
