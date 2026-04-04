/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:10:42 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/04 15:17:33 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
from canon to raw mode + disable echo
-> CTRL + C = "^C" instead of shut down the program
-> echo = disable original echo for use ours
*/
int term_raw_mode(struct termios *oldt, struct termios *newt)
{
	if (tcgetattr(STDIN_FILENO, oldt) == -1)
	{
		perror("minishell: tcgetattr");
    	return (1);
		/* comment gerer ici ?
		erreur terminal = quoi ? */
	}
	newt = oldt;
	
	/* avec echo */
	 newt->c_lflag &= ~(ICANON);
	/* sans echo */
	/*newt->c_lflag &= ~(ICANON | ECHO);*/

	if (tcsetattr(STDIN_FILENO, TCSANOW, newt) == -1) 
	{
		perror("minishell: tcsetattr");
   		return (1);
		/*que faire de cette erreure ? */
	}
	return (0);
}
