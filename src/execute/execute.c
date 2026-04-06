/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:11:38 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/06 21:07:24 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_sigint(int sig)
{
    /*struct termios oldt, newt;*/
	/*t_token	*first_token;*/

	/*first_token = NULL;*/
	(void)sig;
	write(1, "\nminishell$ ", 12); // réaffiche le prompt
}

int execute(t_minishell *minishell)
{
	// t_exec	exec;

	// ft_printf_fd(2, "%s\n", token->value);
	
	// char c;
	// struct termios oldt, newt;

	/* ENV A CHARGER EN AMONT */
	// init_struct_env(env); /*comportemement si on ne trouve pas d user ou 

	is_cmd(minishell);
	
	/*  BOUCLE WHILE  */
	// signal(SIGINT, handle_sigint);
	
	//variables pour boucle while :
	char	*line;
	t_token	*first_token;

	first_token = NULL;

	while (1)
	{
		/*line = readline(env->username);*/
		line = readline("minishell$ ");
		if (!line)
			break;

		if (*line)
			add_history(line);

		// PARSING ICI :************************
		separate_into_tokens(line, &first_token);
		parse_tokens(&first_token);
		ft_token_lstclear(&first_token);
		// *************************************
		// read(STDIN_FILENO, &c, 1);

		// printf("Tu as tapé : %c\n", c);

		free(line);
	}

	

	// revenir au terminal normal
	
	return (0);
	// free_struct_env(&env);
}
