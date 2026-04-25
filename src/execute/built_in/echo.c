/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:27:48 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/25 19:28:04 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



// int index_word(char *src)
// {
// 	char	*echo = "echo ";
// 	int		i;
// 	int		j;

// 	i = 0;
// 	j = 0;
// 	while (src[i])
// 	{
// 		if (src[i + j] == echo[j])
// 			j++;
// 		else
// 		{
// 			j = 0;
// 			i++;
// 		}
// 		if (!echo[j])
// 			return (i + j);
// 	}
// 	return (0);
// }

void	echo_content(t_minishell *minishell, t_token *args)
{
	char	*result;
	char	*tmp;
	int	i;

	i = 0;
	tmp = ft_calloc(1, sizeof(char));
	result = NULL;
	while (args && (minishell->exec.index_pipe == 0 || i < minishell->exec.index_pipe))
	{
		if (args->type == IS_INPUT || args->type == IS_OUTPUT)
			break;
		else
		{
			tmp = ft_strdup(result);
			free(result);
			result = ft_strjoin(tmp, args->value);
			free(tmp);
		}
		args = args->next;
	}
	ft_strlcpy(minishell->builtin.echo.result, result, ft_strlen(result) + 1);
	// il faudra free
}

// int	count_strchr()
// {
// 	while (args)
// 	{
		
// 	}
// 	ft_strncmp()
// }

/*
----- + n = print on new line
- + nnnnnnn = print before prompt
----- + nnnnnn = print on new line
*/
int	echo_for_prompt(t_token *args)
{
	int	i;
	int	minus;
	int	alpha;

	i = 0;
	minus = 0;
	alpha = 0;
	if (args->value[i] == '-')
	{
		minus++;
		while (args->value[i] && args->value[i] == '-')
		{
			minus++;
			i++;
		}
		if (args->value[i] == 'n')
		{
			alpha++;
			while (args->value[i] && args->value[i] == 'n')
			{
				alpha++;
				i++;
			}
		}
	}
	printf("minus = %d\n", minus);
	printf("alpha = %d\n", alpha);
	if (minus > 1)
		return (0);
	else if (minus == 1 && alpha >= 1)
		return (1);
	return (0);
}


// ECHO ****************
/*- print a given string
conditions

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

	i = 0;
	args = NULL;
	if (pipe->cmd->next)
		args = pipe->cmd->next;
	else
		return (0);
	len = ft_strlen(args->value);
	// if (ft_strncmp(args->value, "-n", len + 1) == 0)
	// {
		// si plusieurs - devient normal, pas d effet particulier
		// si pusieurs n garde l effet de print devant le prompt
		// si il y aplusieurs -n espace (avce un espace) a la suite ne pas les print, juste print les mots apres
	if (echo_for_prompt(args))
	{
		echo_content(minishell, args->next);
		return (0);
		
	}
	// }
	while (args && (minishell->exec.index_pipe == 0 || i < minishell->exec.index_pipe))
	{
		len = ft_strlen(args->value);
		if (ft_strnstr(args->value, "$?", len + 1) != NULL)
		{
			j = 0;
			while (args->value[j])
			{
				if (args->value[j] == '$' && args->value[j + 1] == '?')
				{
					ft_printf_fd(1, "%d", minishell->exec.error);
					j += 2;
					continue;
				}
				ft_printf_fd(1, "%c", args->value[j]);
				j++;
			}
		}
		else if (args->type == IS_INPUT || args->type == IS_OUTPUT)
			break;
		else
			ft_printf_fd(1, "%s", args->value);
		// ft_printf_fd(2, "%d", args->type);
		args = args->next;
		i++;
	}
	ft_printf_fd(1, "\n");
	return (0);
}
