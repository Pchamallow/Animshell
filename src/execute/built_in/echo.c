/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:27:48 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/14 13:01:07 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo_skip_options(char *str)
{
	int	i;
	int	option;

	i = 0;
	option = 0;
	if (!ft_strcmp(str, "-n"))
		return (1);
	if (str[i] && str[i] == '-')
	{
		i++;
		while (str[i] && str[i] == 'n')
		{
			option++;
			i++;
		}
	}
	if (option)
		return (1);
	return (0);
}

/*
Content print by echo before the prompt

while 
- value = space
- value = -n (even if we have multiple - followed by multiple n) 
-> we skip

since the first word, the result is keep in echo.result
*/
void	echo_content(t_minishell *minishell, t_token *args)
{
	char	*result;
	char	*tmp;
	int		i;
	int		is_content;

	i = minishell->exec.index_prev_pipe;
	is_content = 0;
	result = ft_strdup("");
	while ((args->type == ONE_SPACE
		|| echo_skip_options(args->value)) && args && args->next)
	{
		args = args->next;
		i++;
	}
	while (args && (minishell->exec.index_pipe == 0 || i < minishell->exec.index_pipe))
	{
		if (args->type == IS_INPUT || args->type == IS_OUTPUT)
			break;
		else 
		{
			tmp = ft_strdup(result);
			if (!tmp)
				print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
			free(result);
			result = ft_strjoin(tmp, args->value);
			if (!result)
				print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
			free(tmp);
			is_content = 1;
		}
		args = args->next;
		i++;
	}
	if (is_content)
	{
		minishell->builtin.echo.result = ft_calloc(ft_strlen(result) + 2, sizeof(char));
		if (!minishell->builtin.echo.result)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		ft_strlcpy(minishell->builtin.echo.result, result, ft_strlen(result) + 1);
		minishell->builtin.echo.for_prompt = true;
	}
	free(result);
}

/*
if we have -n before a word
we print on top of the prompt

----- + n = print on new line
- + nnnnnnn = print before prompt
----- + nnnnnn = print on new line
if we print before prompt, we skip every -n
*/
void	echo_for_prompt(t_minishell *minishell, t_pipe *pipe)
{
	t_token *args;

	if (pipe->cmd->next)
	{
		args = pipe->cmd->next;
		if (count_chr(&args->value[0], '-', true) == 1)
		{
			if (count_chr(&args->value[1], 'n', true) >= 1)
			{
				if (args->next)
					echo_content(minishell, args->next);
			}
		}
	}
}

void	print_no_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '"')
			ft_printf_fd(1, "%c", str[i]);
		i++;
	}
}

void	echo_args(t_minishell *minishell, t_token *args)
{
	int	i;

	i = minishell->exec.index_prev_pipe;
	while (args)
	{
		if (args->type == PIPE)
			break ;
		else if (args->type > REDIRECTION)
			ft_printf_fd(1, " ");
		else if (args->type < REDIRECTION
			&& !args->file_input && !args->file_output)
			ft_printf_fd(1, "%s", args->value);
		args = args->next;
		i++;
	}
}

int	ft_iswhitespaces(char c)
{
	if ((c >= 8 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int	echo_args_whitespaces(t_token *args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	// if (!args)
	// 	return (0);
	while (args)
	{
		j = 0;
		while (args->value[j])
		{
			if (!ft_iswhitespaces(args->value[j]))
				return (0);
			j++;
		}
		args = args->next;
		i++;
	}
	return (1);
}

// ECHO ****************
/*- print a given string
conditions

- with argument -n

- exit status to print
- - if $? is find, 
- - if $?word or word$? 
- - no print with $ ?*/
// *********************
int echo(t_minishell *minishell, t_pipe *pipe)
{
	t_token	*args;

	args = NULL;
	if (minishell->builtin.echo.for_prompt == true)
		return (0);
	if (pipe->cmd->next)
	{
		args = pipe->cmd->next;
		if (!echo_args_whitespaces(args))
			echo_args(minishell, args);
	}
	ft_printf_fd(1, "\n");
	return (0);
}
