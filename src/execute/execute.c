/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:11:38 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/04 14:16:05 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execute(t_env *env)
{
    struct termios oldt, newt;

	/* ENV A CHARGER EN AMONT */
	init_struct_env(env); /*comportemement si on ne trouve pas d user ou 
	de name ?*/
	// ft_printf_fd(2, "Your username is %s\n", env.username);

	if (term_raw_mode(&oldt, &newt))
		return (1);
	
	/*  BOUCLE WHILE  */
	char *line;
	// char	c;


	// ft_printf_fd(2, "%d\n", nb);
	
	// ft_printf_fd(2, "%s\n", env.username);
	
	while (1)
	{
		line = readline(env->username);
		if (!line)
			break;

		if (*line)
			add_history(line);

		// read(STDIN_FILENO, &c, 1);

		// printf("Tu as tapé : %c\n", c);

		free(line);
	}

	

	// revenir au terminal normal
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	
	return (0);
	// free_struct_env(&env);
}
