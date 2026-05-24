/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:27:48 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/24 18:07:13 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*add_arg(t_minishell *minishell, t_token *args,
		char **result, bool *is_arg)
{
	char	*tmp;

	if (*is_arg == true)
		tmp = ft_strjoin(*result, " ");
	else
		tmp = ft_strdup(*result);
	if (!tmp)
	{
		free(*result);
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
	free(*result);
	*result = ft_strjoin(tmp, args->value);
	free(tmp);
	if (!*result)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	*is_arg = true;
	return (*result);
}

static void	update_echo_result(t_minishell *minishell, char *result)
{
	minishell->builtin.echo.result = ft_calloc(ft_strlen(result) + 2,
			sizeof(char));
	if (!minishell->builtin.echo.result)
	{
		free(result);
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
	ft_strlcpy(minishell->builtin.echo.result, result,
		ft_strlen(result) + 1);
	minishell->builtin.echo.for_prompt = true;
}

static void	echo_args_for_prompt(t_minishell *minishell, t_token *args,
	char **result, bool *is_arg)
{
	int	i;

	i = 0;
	while (args)
	{
		if (args->type == PIPE)
			break ;
		else if (args->type == IS_ARG)
			*result = add_arg(minishell, args, result, is_arg);
		args = args->next;
		i++;
	}
}

/*
Content print by echo before the prompt

while 
- value = -n (even if we have multiple - followed by multiple n) 
-> we skip

since the first word, the result is kept in echo.result
*/
static void	echo_content(t_minishell *minishell, t_token *args)
{
	char	*result;
	int		i;
	bool	is_arg;

	i = 0;
	is_arg = false;
	result = ft_strdup("");
	if (!result)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	while ((echo_is_option(args->value)) && args && args->next)
	{
		args = args->next;
		i++;
	}
	echo_args_for_prompt(minishell, args, &result, &is_arg);
	if (is_arg == true)
		update_echo_result(minishell, result);
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
void	echo(t_minishell *minishell, t_pipe *pipe)
{
	t_token	*args;

	if (pipe->cmd->next)
	{
		args = pipe->cmd->next;
		if (count_chr(&args->value[0], '-', true) == 1)
		{
			if (count_chr(&args->value[1], 'n', true) >= 1)
			{
				if (echo_is_option(args->value) && args->next)
					echo_content(minishell, args->next);
			}
		}
	}
}
