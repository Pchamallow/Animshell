/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 11:41:13 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/08 23:29:30 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	found_unset_var(char *env_var, t_token *arg)
{
	int	i;

	while (arg)
	{
		i = 0;
		while (env_var[i] && env_var[i] != '=' && arg->value[i])
		{
			if (env_var[i] != arg->value[i])
				break ;
			i++;
			if ((!env_var[i] || env_var[i] == '=') && !arg->value[i])
				return (true);
		}
		arg = arg->next;
	}
	return (false);
}

char	**copy_unset_envp(t_minishell *minishell, t_token *arg, int envp_len)
{
	int		i;
	int		j;
	char	**new_envp;

	new_envp = ft_calloc(envp_len + 1, sizeof(char *));
	if (!new_envp)
		print_error_free(minishell, "malloc error in export\n", EXIT_FAILURE);
	i = 0;
	j = 0;
	while (minishell->exec.envp[i])
	{
		if (found_unset_var(minishell->exec.envp[i], arg))
		{
			free(minishell->exec.envp[i]);
			i++;
		}
		else
		{
			new_envp[j] = minishell->exec.envp[i];
			j++;
			i++;
		}
	}
	new_envp[j] = NULL;
	return (new_envp);
}

/*
** UNSET ****************************************
** - no options, as requested by the subject
** - always returns 0
** - if variable(s) @arg->value is/are found in @minishell->exec.envp:
**		- creates a char **@new_envp
**		- copies @minishell->exec.envp to @new_envp, removing found variables
**	*********************************************
*/
int	unset(t_minishell *minishell, t_pipe *pipe)
{
	t_token	*arg;
	int		count;
	int		envp_len;
	char	**new_envp;

	minishell->exec.error = 0;
	if (pipe->nb_args == 0)
		return (minishell->exec.error);
	arg = pipe->cmd->next;
	count = 0;
	while (arg)
	{
		if (found_var(minishell->exec.envp, arg->value) >= 0)
			count++;
		arg = arg->next;
	}
	if (count == 0)
		return (minishell->exec.error);
	envp_len = get_envp_len(minishell->exec.envp) + count;
	new_envp = copy_unset_envp(minishell, pipe->cmd->next, envp_len);
	free (minishell->exec.envp);
	minishell->exec.envp = new_envp;
	return (minishell->exec.error);
}
