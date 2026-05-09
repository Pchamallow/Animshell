/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 14:11:33 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/09 16:39:14 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*new_concat(char *arg)
{
	char	*str;
	int		i;
	int		j;
	int		len;

	len = ft_strlen(arg);
	str = ft_calloc(len, sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (arg[i])
	{
		if (arg[i] != '+')
		{
			str[j] = arg[i];
			j++;
		}
		i++;
	}
	return (str);
}

bool	is_unique(t_token *arg)
{
	t_token	*first_arg;

	first_arg = arg;
	arg = arg->next;
	while (arg)
	{
		if (is_same_name(first_arg->value, arg))
			return (false);
		arg = arg->next;
	}
	return (true);
}

int	count_lines_to_add(t_minishell *minishell, t_token *arg,
		bool *valid_var_to_add)
{
	int	count;

	count = 0;
	while (arg && arg->type != PIPE)
	{
		if (arg->type == IS_ARG && !is_valid_arg(arg->value))
		{
			ft_printf_fd(2, "minishell: export: \'%s\' :", arg->value);
			ft_printf_fd(2, " not a valid identifier\n");
			minishell->exec.error = 1;
		}
		else if (arg->type == IS_ARG && is_valid_arg(arg->value))
		{
			*valid_var_to_add = true;
			if (found_var(minishell->exec.envp, arg->value) < 0
				&& is_unique(arg))
				count++;
		}
		arg = arg->next;
	}
	return (count);
}

bool	export_option_error(t_minishell *minishell, t_token *arg)
{
	if (arg->type == IS_ARG && arg->value[0] == '-' && arg->value[1] != '\0')
	{
		ft_putstr_fd("minishell: export: -", 2);
		write(2, &arg->value[1], 1);
		ft_putstr_fd(" : invalid option\n", 2);
		ft_putstr_fd("export: usage: export [nom[=valeur] ...]\n", 2);
		minishell->exec.error = 2;
		return (true);
	}
	return (false);
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
		print_error_free(minishell, "malloc error in export\n", 1);
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

/*
** EXPORT WITH ARGUMENTS ************************
** - no options, as requested by the subject
** - adds valid variables in char **envp
** ! if export is called without arguments :
**		-> see export_print function
**	*********************************************
*/
int	export(t_minishell *minishell, t_pipe *pipe)
{
	t_token	*arg;
	int		count;
	char	**new_envp;
	bool	valid_var_to_add;

	minishell->exec.error = 0;
	if (pipe->nb_args == 0)
		return (minishell->exec.error);
	arg = pipe->cmd->next;
	if (export_option_error(minishell, arg))
		return (minishell->exec.error);
	valid_var_to_add = false;
	count = count_lines_to_add(minishell, arg, &valid_var_to_add);
	if (pipe->next)
		return (minishell->exec.error);
	if (valid_var_to_add)
	{
		new_envp = update_envp(minishell, arg, count);
		free(minishell->exec.envp);
		minishell->exec.envp = new_envp;
	}
	return (minishell->exec.error);
}
