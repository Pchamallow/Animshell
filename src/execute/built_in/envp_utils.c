/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 15:18:00 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/09 17:18:57 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env_cmp(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;
	unsigned char	c1;
	unsigned char	c2;

	i = 0;
	str1 = (unsigned char *) s1;
	str2 = (unsigned char *) s2;
	c1 = str1[i];
	c2 = str2[i];
	while (c1 || c2)
	{
		if (c1 == '=')
			c1 = 1;
		if (c2 == '=')
			c2 = 1;
		if (c1 != c2)
			return (c1 - c2);
		i++;
		c1 = str1[i];
		c2 = str2[i];
	}
	return (0);
}

int	get_envp_len(char **envp)
{
	int	i;

	if (!envp)
		return (0);
	i = 0;
	while (envp[i])
		i++;
	return (i);
}

bool	is_concat(char *arg, int i)
{
	if (arg[i] == '+' && arg[i + 1] == '=')
		return (true);
	return (false);
}

int	concat_found(char *arg)
{
	int		i;

	i = 0;
	while (arg[i] && arg[i] != '=' && arg[i] != '+')
		i++;
	if (!arg[i] || !is_concat(arg, i))
		return (-1);
	return (i);
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
