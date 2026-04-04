/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 11:00:00 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/04 11:49:20 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int    get_username(char **str)
{
    const char *user;
	const char *name;
	int len_user;
	int len_name;

	user = getenv("USER");
	name = getenv("NAME");
	if (!user || !name)
		return (1);
	len_user = ft_strlen(user);
	len_name = ft_strlen(name);
	(*str) = ft_calloc((len_user + len_name + 4), sizeof(char));
	ft_strlcpy((*str), user, len_user + 1);
	(*str)[len_user] = '@';
	ft_strlcpy(&(*str)[len_user + 1], name, len_name + 1);
	(*str)[len_user + len_name + 1] = '$';
	(*str)[len_user + len_name + 2] = ' ';
	return (0);
}

void	free_struct_env(t_env *env)
{
	if (env->username)
		free(env->username);
}

/* get_username = for the prompt we have "username@hostname$ "
*/
void    init_struct_env(t_env *env)
{
	env->username = NULL;
    get_username(&env->username);
}