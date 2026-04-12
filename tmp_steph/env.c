/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 18:54:07 by stkloutz          #+#    #+#             */
/*   Updated: 2026/04/09 19:00:05 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include "minishell.h"*/

#include<stdio.h>

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	
	int	i;

	i = 0;
	while (envp[i])
	{
		/*ft_printf_fd(1, "%s\n", envp[i]);*/
		printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}
