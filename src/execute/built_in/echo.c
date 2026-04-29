/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:27:48 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/29 09:30:10 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
dst = src only ?
examples :
-------nnnnnn = true
-----nnnannnnn = false
*/
bool	only_str(char *dst, char *src)
{
	int	i;

	i = 0;
	while (dst[i])
	{
		if (dst[i] != src[0] && dst[i] != src[1])
			return (false);
		i++;
	}
	return (true);
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
		|| only_str(args->value, "-n")) && args && args->next)
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
	int		i;
	int		j;
	int		len;
	// int		quoted;

	i = 0;
	// quoted = 0;
	args = NULL;
	if (pipe->cmd->next)
		args = pipe->cmd->next;
	else
		return (0);
	if (minishell->builtin.echo.for_prompt == true)
		return (0);
	while (args && (minishell->exec.index_pipe == 0
		|| i < minishell->exec.index_pipe))
	{
		len = ft_strlen(args->value);
		if (args->type == IS_INPUT || args->type == IS_OUTPUT
			|| args->type == IS_APPEND || args->type == HEREDOC)
			break;
		else if (ft_strnstr(args->value, "$?", len + 1) != NULL)
		{
			j = 0;
			// quoted = false;
			printf("quotes = %d\n", args->quote);
			while (args->value[j])
			{
				// if (args->value[j] == '\'' || args->value[j] == '"')
				// 	quoted = true;
				if ((args->quote == NO || args->quote == DOUBLE)
					&& args->value[j] == '$' && args->value[j + 1] == '?')
				{
					ft_printf_fd(1, "%d", minishell->exec.error);
					j += 2;
					continue;
				}
				if (args->value[j] != '"')
					ft_printf_fd(1, "%c", args->value[j]);
				j++;
			}
		}
		else
			print_no_quotes(args->value);
		// ft_printf_fd(2, "%d", args->type);
		args = args->next;
		i++;
	}
	ft_printf_fd(1, "\n");
	return (0);
}
