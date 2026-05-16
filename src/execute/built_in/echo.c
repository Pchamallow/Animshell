/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:27:48 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/15 17:36:39 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** options	=	-n	-nnnnnnnnn 
** not		=	-	n
*/
int	echo_is_option(char *str)
{
	bool	option;
	int		i;

	i = 0;
	option = false;
	if (str[i] == '-')
	{
		i++;
		while (str[i] && str[i] == 'n')
		{
			option = true;
			i++;
		}
	}
	if (!str[i] && option == true)
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

	i = 0;
	is_content = 0;
	result = ft_strdup("");
	while ((args->type == ONE_SPACE
		|| echo_is_option(args->value)) && args && args->next)
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

int	ft_iswhitespaces(int c)
{
	if ((c >= 7 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int	echo_args_iswhitespaces(t_token *args)
{
	int	i;

	i = 0;
	while (args)
	{
		while(args->value[i])
		{
			if (!ft_iswhitespaces(args->value[i]))
				return (0);
			i++;
		}
		args = args->next;
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
	t_token *args;
	bool	is_arg;

	args = NULL;
	is_arg = false;
	if (minishell->builtin.echo.for_prompt == true)
		return (0);
	if (pipe->cmd->next && pipe->cmd->next->type != PIPE)
	{
		args = pipe->cmd->next;
		if (echo_args_iswhitespaces(args))
		{
			ft_printf_fd(1, "\n");
			return (0);
		}
		while (args) // ajouter de skip les spaces
		{
			if (args->type == PIPE)
				return (0);
			if (args->type == IS_ARG)
			{
				if (is_arg == true)
					ft_printf_fd(1, " ");
				ft_printf_fd(1, "%s", args->value);
				is_arg = true;
			}
			args = args->next;
		}
	}
	ft_printf_fd(1, "\n");
	return (0);
}


// skip les infiles
// si pas utiliser skip les outfiles 
// faire 1 espace


