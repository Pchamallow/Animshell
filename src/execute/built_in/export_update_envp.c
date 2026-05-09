/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_update_envp.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 17:11:36 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/09 17:13:30 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	concat_var(char **envp, char *var, t_minishell *minishell)
{
	int		i;
	int		j;
	char	*tmp;

	i = found_var(envp, var);
	if (i < 0 || !ft_env_cmp(envp[i], var) || !ft_strchr(var, '='))
		return ;
	j = concat_found(var);
	if (ft_strchr(envp[i], '='))
		j += 2;
	else
		j += 1;
	tmp = safe_join(envp[i], var + j);
	if (!tmp)
	{
		free(envp);
		print_error_free(minishell, "malloc error in export\n", EXIT_FAILURE);
	}
	free(envp[i]);
	envp[i] = tmp;
}

void	replace_var(char **envp, char *var, t_minishell *minishell)
{
	int		i;

	i = found_var(envp, var);
	if (i < 0 || !ft_env_cmp(envp[i], var) || !ft_strchr(var, '='))
		return ;
	free(envp[i]);
	envp[i] = ft_strdup(var);
	if (!envp[i])
	{
		free(envp);
		print_error_free(minishell, "malloc error in export\n", EXIT_FAILURE);
	}
}

void	update_var(t_minishell *minishell, char **new_envp, char *arg)
{
	if (concat_found(arg) >= 0)
		concat_var(new_envp, arg, minishell);
	else
		replace_var(new_envp, arg, minishell);
}

void	create_var(t_minishell *minishell, char **new_envp, char *arg, int i)
{
	if (concat_found(arg) >= 0)
		new_envp[i] = new_concat(arg);
	else
		new_envp[i] = ft_strdup(arg);
	if (!new_envp[i])
	{
		free_strv(new_envp);
		print_error_free(minishell, "malloc error in export\n", EXIT_FAILURE);
	}
}

char	**update_envp(t_minishell *minishell, t_token *arg, int count)
{
	int		i;
	char	**new_envp;

	i = get_envp_len(minishell->exec.envp);
	new_envp = envp_copy(minishell->exec.envp, count + i);
	if (!new_envp)
		print_error_free(minishell, "malloc error in export\n", EXIT_FAILURE);
	while (arg)
	{
		if (arg->type == IS_ARG && is_valid_arg(arg->value)
			&& found_var(new_envp, arg->value) >= 0)
			update_var(minishell, new_envp, arg->value);
		else if (arg->type == IS_ARG && is_valid_arg(arg->value))
		{
			create_var(minishell, new_envp, arg->value, i);
			i++;
		}
		arg = arg->next;
	}
	new_envp[i] = NULL;
	return (new_envp);
}
