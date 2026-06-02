/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 17:22:40 by stkloutz          #+#    #+#             */
/*   Updated: 2026/06/02 15:23:36 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** is_valid_arg checks for:
** valid variable name (name = part before '=' or '+='):
**	- starts with an alphabetical character or '_'
**	- contains only alphanumerical characters or '_'
** valid variable value:
**	- any printable characters
*/
bool	is_valid_arg(char *arg)
{
	size_t	i;

	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (false);
	i = 1;
	while (arg[i] && arg[i] != '=' && !is_concat(arg, i))
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (false);
		i++;
	}
	while (arg[i])
	{
		if (!ft_isprint(arg[i]))
			return (false);
		i++;
	}
	return (true);
}

char	*new_concat(char *arg)
{
	char	*str;
	int		i;
	int		j;
	int		len;
	int		plus;

	len = ft_strlen(arg);
	str = ft_calloc(len, sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	plus = strfind(arg, "+");
	while (arg[i])
	{
		if (i != plus)
		{
			str[j] = arg[i];
			j++;
		}
		i++;
	}
	return (str);
	//suppr original en dessous ou test ?
}


// original
// char	*new_concat(char *arg)
// {
// 	char	*str;
// 	int		i;
// 	int		j;
// 	int		len;

// 	len = ft_strlen(arg);
// 	str = ft_calloc(len, sizeof(char));
// 	if (!str)
// 		return (NULL);
// 	i = 0;
// 	j = 0;
// 	while (arg[i])
// 	{
// 		if (arg[i] != '+')
// 		{
// 			str[j] = arg[i];
// 			j++;
// 		}
// 		i++;
// 	}
// 	return (str);
// }
