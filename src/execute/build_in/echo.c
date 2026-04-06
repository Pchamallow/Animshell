/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:27:48 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/04 16:04:27 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



int index_word(char *src)
{
	char	*echo = "echo ";
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (src[i])
	{
		if (src[i + j] == echo[j])
			j++;
		else
		{
			j = 0;
			i++;
		}
		if (!echo[j])
			return (i + j);
	}
	return (0);
}


/*
echo
- print a given string
conditions

*/
int echo(t_exec *exec)
{
	char	*tmp;
	char	*result;
	int		i;
	int		index;

	// if (exec.echo = 1)
	i = 0;
	tmp = ft_strnstr(exec->line, "echo ",
		ft_strlen(exec->line));
	index = index_word(&exec->line[i]);
	result = ft_substr(exec->line, index, ft_strlen(tmp) + 1);
	ft_printf_fd(STDOUT_FILENO, "%s\n", result);
	return (0);
}

/*
faire la verification de cmd, voir si elle passe,
si el chemin est explicite ou non etc
-> test des cmd, voir si word = echo 
-> exec.echo = 1 si il y a "echo " + un mot derriere
voir les diffrents contextes d utilisation d echo 

parsing verifier les mots les uns apres les autres
que ce soit bien pas des commandes


j ai besoin dune structure ? ou pas 

mot1

cmd1

mot2

cmd2


ici pour dire a echo print les mots d apres
mais arrete s il autre chose qu un mot
= recuperer le type

*/