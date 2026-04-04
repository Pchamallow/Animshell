/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:07:23 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/04 12:08:44 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
from canon to raw mode + turn off echo
-> CTRL + C = "^C" instead of shut down the program
-> echo = turn off original echo for use ours
*/
int term_raw_mode(struct termios oldt, struct termios newt)
{
	if (tcgetattr(STDIN_FILENO, &oldt) == -1)
	{
		perror("minishell: tcgetattr");
    	return (1);
		/* comment gerer ici ?
		erreur terminal = quoi ? */
	}
	newt = oldt;
	// newt.c_lflag &= ~(ICANON);
	newt.c_lflag &= ~(ICANON | ECHO);

	if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == -1) 
	{
		perror("minishell: tcsetattr");
   		return (1);
		/*que faire de cette erreure ? */
	}
	return (0);
}



int	main(void)
{
	struct termios oldt, newt;
	t_env env;

	/* ENV A CHARGER EN AMONT */
	init_struct_env(&env);
	/*comportemement si on ne trouve pas d user ou 
	de name ?*/
	// ft_printf_fd(2, "Your username is %s\n", env.username);

	if (term_raw_mode(oldt, newt))
		return (1);
	
	/*  BOUCLE WHILE  */
	char *line;
	// char	c;


	// ft_printf_fd(2, "%d\n", nb);
	
	while (1)
	{
		line = readline(env.username);
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
	
	free_struct_env(&env);
	
	return 0;
}
