/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 22:01:51 by stkloutz          #+#    #+#             */
/*   Updated: 2026/03/06 20:52:03 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minitalk.h"

volatile sig_atomic_t	g_ack_received = 0;

void	handle_sigusr(int sig)
{
	(void)sig;
	g_ack_received = 1;
}

void	send_bit(int server_pid, int bit_value)
{
	int	timer;
	int	time_sleep;

	g_ack_received = 0;
	if (bit_value == 0)
		kill(server_pid, SIGUSR1);
	else
		kill(server_pid, SIGUSR2);
	timer = 2000000;
	time_sleep = 100;
	while (g_ack_received == 0)
	{
		usleep(time_sleep);
		timer -= time_sleep;
		if (timer <= 0)
		{
			ft_putendl_fd(ERR_COMM, 2);
			exit(1);
		}
	}
	usleep(50);
}

void	send_char(char c, int server_pid)
{
	int	bit_count;
	int	bit_value;

	bit_count = 7;
	while (bit_count >= 0)
	{
		bit_value = (c >> bit_count) & 1;
		send_bit(server_pid, bit_value);
		bit_count--;
	}
}

int	main(int ac, char **av)
{
	int					server_pid;
	int					i;
	struct sigaction	sa;

	server_pid = 0;
	if (!check_input(ac, av, &server_pid))
		return (1);
	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = &handle_sigusr;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	if (sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		ft_putendl_fd(ERR_SIG, 2);
		return (1);
	}
	i = 0;
	while (av[2][i])
	{
		send_char(av[2][i], server_pid);
		i++;
	}
	send_char('\0', server_pid);
	return (0);
}
