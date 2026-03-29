/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 20:21:11 by stkloutz          #+#    #+#             */
/*   Updated: 2026/03/07 08:59:26 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minitalk.h"

void	init_sigaction(t_sigaction *sa)
{
	sa->sigusr.sa_flags = SA_SIGINFO;
	sa->sigusr.sa_sigaction = &handle_sigusr_server;
	sigemptyset(&sa->sigusr.sa_mask);
	sigaddset(&sa->sigusr.sa_mask, SIGUSR1);
	sigaddset(&sa->sigusr.sa_mask, SIGUSR2);
	sigaddset(&sa->sigusr.sa_mask, SIGINT);
	sigaddset(&sa->sigusr.sa_mask, SIGTERM);
	sa->sigterm.sa_handler = &handle_sigterm;
	sigemptyset(&sa->sigterm.sa_mask);
	sigaddset(&sa->sigterm.sa_mask, SIGUSR1);
	sigaddset(&sa->sigterm.sa_mask, SIGUSR2);
	sigaddset(&sa->sigterm.sa_mask, SIGINT);
	sigaddset(&sa->sigterm.sa_mask, SIGQUIT);
}

void	add_msg(t_list **msg)
{
	char	*str;
	t_list	*new_node;

	str = ft_calloc(LEN_LIMIT, sizeof(char));
	if (!str)
	{
		ft_putendl_fd(ERR_MALLOC, 2);
		ft_lstclear(msg, free);
		exit(1);
	}
	new_node = ft_lstnew(str);
	if (!new_node)
	{
		ft_putendl_fd(ERR_MALLOC, 2);
		ft_lstclear(msg, free);
		exit(1);
	}
	ft_lstadd_back(msg, new_node);
}

void	print_str(t_list *msg)
{
	while (msg)
	{
		ft_printf("%s", (char *)msg->content);
		msg = msg->next;
	}
	ft_printf("\n");
}
