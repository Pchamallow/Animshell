/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 14:11:33 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/02 22:48:55 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_strcmp(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	i = 0;
	str1 = (unsigned char *) s1;
	str2 = (unsigned char *) s2;
	while (str1[i] || str2[i])
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}

static char	**envp_copy(char **envp)
{
	int		i;
	int		len;
	char	**new_envp;

	len = 0;
	while (envp[len])
		len++;
	new_envp = ft_calloc(len + 1, sizeof(char*));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_envp[i] = envp[i];
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

static void	sort_envp(char **sorted_envp)
{
	int		i;
	int		j;
	int		smallest;
	char	*tmp;

	i = 0;
	while (sorted_envp[i])
	{
		smallest = i;
		j = i + 1;
		while (sorted_envp[j])
		{
			if (ft_strcmp(sorted_envp[smallest], sorted_envp[j]) > 0)
				smallest = j;
			j++;
		}
		if (smallest != i)
		{
			tmp = sorted_envp[i];
			sorted_envp[i] = sorted_envp[smallest];
			sorted_envp[smallest] = tmp;
		}
		i++;
	}
}

static int	print_sorted_envp(t_minishell *minishell)
{
	int		i;
	int		j;
	char	**sorted_envp;

	if (!minishell->exec.envp)
		return (0);
	sorted_envp = envp_copy(minishell->exec.envp);
	if (!sorted_envp)
		print_error_free(minishell, "malloc error in export\n", EXIT_FAILURE);
	sort_envp(sorted_envp);
	i = 0;
	while (sorted_envp[i])
	{
		j = 0;
		write(1, "export ", 7);
		while (sorted_envp[i][j] && sorted_envp[i][j] != '=')
		{
			write(1, &sorted_envp[i][j], 1);
			j++;
		}
		if (sorted_envp[i][j] == '=')
		{
			write(1, "=\"", 2);
			j++;
			write(1, sorted_envp[i] + j, ft_strlen(sorted_envp[i] + j));
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
		i++;
	}
	free(sorted_envp);
	return (0);
}

/*
** is_valid_arg checks for:
** valid variable name (name = part before '='):
**	- starts with '_' or alphabetical characters
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
	while (arg[i] && arg[i] != '=')
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
		while (envp[i][j] && envp[i][j] != '=')
		{
			if (envp[i][j] != var[j])
				break ;
			j++;
			if ((envp[i][j] == '=' || !envp[i][j]) && (var[j] == '=' || !var[j]))
				return (i);
		}
		i++;
	}
	return (-1);
}

void	replace_var(char **envp, char *var, t_minishell *minishell)
{
	int	i;

	i = found_var(envp, var);
	if (i < 0)
		return ;
	if (ft_strcmp(envp[i], var) == 0)
		return ;
	if (!ft_strchr(var, '='))
		return ;
	free(envp[i]);
	envp[i] = ft_strdup(var);
	if (!envp[i])
	{
		free_strv(envp);
		print_error_free(minishell, "malloc error in export\n", EXIT_FAILURE);
	}
}

/*
** EXPORT ***************************************
** - no options, as requested by the subject
** - if no arguments :
**		- lists all environment variables
**		- in alphabetical order
**		- preceeded with "export "
**		- the variable value is enclosed with quotes
** - else : adds variables in char **envp
**	*********************************************
*/
int export(t_minishell *minishell, t_pipe *pipe)
{
	t_token	*arg;
	int		count;
	int		i;
	char	**new_envp;
	bool	valid_var_to_add;

	minishell->exec.error = 0;
	if (pipe->nb_args == 0)
	{
		print_sorted_envp(minishell);
		return (minishell->exec.error);
	}
	arg = pipe->cmd->next;
	valid_var_to_add = false;
	//erreur si option:
	if (arg->type == IS_ARG && arg->value[0] == '-' && arg->value[1] != '\0')
	{
		ft_printf_fd(2, "minishell: export: -%c :", arg->value[1]);
		ft_printf_fd(2, " invalid option\n");
		ft_printf_fd(2, "export: usage: export [nom[=valeur] ...]\n");
		minishell->exec.error = 2;
		return (minishell->exec.error);
	}
	//compte combien de lignes à rajouter à envp:
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
			valid_var_to_add = true;
			if (found_var(minishell->exec.envp, arg->value) < 0)
				count++;
		}
		arg = arg->next;
	}
	//ajoute les variables valides à envp:
	if (valid_var_to_add)
	{
		i = 0;
		while (minishell->exec.envp[i])
			i++;
		count += i;
		new_envp = ft_calloc(count + 1, sizeof(char*));
		if (!new_envp)
			print_error_free(minishell, "malloc error in export\n", EXIT_FAILURE);
		i = 0;
		while (minishell->exec.envp[i])
		{
			new_envp[i] = minishell->exec.envp[i];
			i++;
		}
		arg = pipe->cmd->next;
		while (arg)
		{
			if (arg->type != IS_ARG || !is_valid_arg(arg->value))
			{
				arg = arg->next;
				continue ;
			}
			if (found_var(new_envp, arg->value) >= 0)
			{
				replace_var(new_envp, arg->value, minishell);
				arg = arg->next;
				continue ;
			}
			new_envp[i] = ft_strdup(arg->value);
			if (!new_envp[i])
			{
				free_strv(new_envp);
				print_error_free(minishell, "malloc error in export\n", EXIT_FAILURE);
			}
			arg = arg->next;
			i++;
		}
		new_envp[i] = NULL;
		free(minishell->exec.envp);
		minishell->exec.envp = new_envp;
	}
	return (minishell->exec.error);
}
