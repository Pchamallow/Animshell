/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 19:11:21 by stkloutz          #+#    #+#             */
/*   Updated: 2026/03/18 17:23:45 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

// INCLUDES:
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <stdbool.h>
# include <limits.h>
# include "../src/libft/libft.h"
# include "../src/libft/printf/ft_printf.h"

//DEFINES:
# define LEN_LIMIT 100
//ERRORS:
# define ERR_COMM "Communication problem with the server"
# define ERR_INT_LIMIT "Error: server PID must be an int between 1 and INT_MAX"
# define ERR_NB_ARG "Error: incorrect number of arguments\n"
# define SYNTAX "Please follow this syntax:\n"
# define USAGE "./client [server PID] \"The string to send\""
# define ERR_SIG "Sigaction Error"
# define ERR_MALLOC "Problem with Memory Allocation"

// TYPEDEFS:

typedef struct s_chr
{
	char	c;
	int		bit_count;
}				t_chr;

typedef struct s_str
{
	int		i;
	t_list	*tmp;
}				t_str;

typedef struct s_sa
{
	struct sigaction	sigusr;
	struct sigaction	sigterm;
}				t_sigaction;

// FUNCTIONS:
// CLIENT:
bool	check_input(int ac, char **av, int *server_pid);
void	handle_sigusr(int sig);
void	send_bit(int server_pid, int bit);
void	send_char(char current, int server_pid);
// SERVER:
void	init_sigaction(t_sigaction *sa);
void	handle_sigterm(int sig);
void	handle_sigusr_server(int sig, siginfo_t *info, void *context);
void	update_str(t_chr *current, t_list **msg, t_str *str);
void	print_str(t_list *msg);
void	add_msg(t_list **msg);
void	get_msg(t_chr *current, t_list *msg, t_str *str);

#endif
