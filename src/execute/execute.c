/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:11:38 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/04 16:17:21 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_sigint(int sig)
{
    (void)sig;
    write(1, "\nminishell$ ", 12); // réaffiche le prompt
}

int execute(t_env *env, char **token)
{
	t_exec	exec;
	// char c;
    // struct termios oldt, newt;

	/* ENV A CHARGER EN AMONT */
	init_struct_env(env); /*comportemement si on ne trouve pas d user ou 
	de name ?*/

	is_cmd(&exec, token);
	
	/*  BOUCLE WHILE  */
	signal(SIGINT, handle_sigint);
	
	while (1)
	{
		exec.line = readline("minishell$ ");
		if (!exec.line)
		{
		    printf("exit\n");
			break;
		}
		
		/*  raw mode */
		// if (term_raw_mode(&oldt, &newt))
		// 	return (1);
		// read(STDIN_FILENO, &c, 1);
		// if (c == 'C')
		// 	return (1);
		// tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
		echo(&exec);
		
		if (*exec.line)
			add_history(exec.line);
			
		free(exec.line);
	}

	

	// revenir au terminal normal
	
	return (0);
	// free_struct_env(&env);
}
