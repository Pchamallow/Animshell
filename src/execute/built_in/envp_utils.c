/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 15:18:00 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/08 15:28:11 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_concat(char *arg, int i)
{
	if (arg[i] == '+' && arg[i + 1] == '=')
		return (true);
	return (false);
}

/*
**	found_var
**	checks if the name of @var already exists in @envp:
**		- browses each line until '='
**		- if @var is encountered it returns @i index of the line
**		- else it returns -1
*/
int	found_var(char **envp, char *var)
{
	int	i;
	int	j;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] != '=' && !is_concat(var, j))
		{
			if (envp[i][j] != var[j])
				break ;
			j++;
			if ((!envp[i][j] || envp[i][j] == '=')
					&& (!var[j] || var[j] == '=' || is_concat(var, j)))
				return (i);
		}
		i++;
	}
	return (-1);
}
