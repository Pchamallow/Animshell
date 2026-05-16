/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 11:21:18 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/16 18:59:43 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlcat_minishell(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (dst)
		i = ft_strlen(dst);
	j = 0;
	while ((i + j) < size && src[j])
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = '\0';
	return (i + j);
}

int	replace_exit_status(char *line, t_expand *expand, t_minishell *minishell)
{
	char	*exit_str;

	exit_str = ft_itoa(minishell->exec.error);
	if (!exit_str)
		error_malloc(line, expand->newline, minishell, "expand: exit status");
	ft_strlcat_minishell(expand->newline, exit_str, expand->count + 1);
	free(exit_str);
	return (2);
}

/*
** replace_var_name
** replaces the $name of ENV VAR
** by its value.
** If the value contains quotes inside
** it encloses each word with quotes
** in order to keep inside quotes.
*/
int	replace_var_name(char *line, t_expand *expand,
		t_minishell *minishell, int i)
{
	int	wd_len;
	int	j;

	wd_len = get_var_name_len(line + i + 1);
	j = get_var(line + i + 1, minishell->exec.envp, wd_len);
	if (j != -1 && minishell->exec.envp[j] != NULL)
	{
		if (quote_found(minishell->exec.envp[j]) && expand->quote == NO)
		{
			ft_strlcat_add_quotes(expand->newline,
				minishell->exec.envp[j] + wd_len + 1, expand->count + 1);
		}
		else if (ft_strchr(minishell->exec.envp[j], '='))
		{
			ft_strlcat_minishell(expand->newline,
				minishell->exec.envp[j] + wd_len + 1, expand->count + 1);
		}
	}
	return (wd_len + 1);
}

void	update_line(char *line, t_minishell *minishell, t_expand *expand)
{
	int	i;
	int	len;

	i = 0;
	while (i < (int)ft_strlen(line))
	{
		len = find_env_var(line + i, ft_strlen(line + i), &expand->quote);
		if (len == -1)
		{
			ft_strlcat_minishell(expand->newline, line + i, expand->count + 1);
			i += ft_strlen(line + i);
		}
		else if (len == 0 && line[i + 1] == '?')
			i += replace_exit_status(line, expand, minishell);
		else if (len == 0 && line[i + 1] != '?')
			i += replace_var_name(line, expand, minishell, i);
		else
		{
			ft_strlcat_minishell(expand->newline, line + i,
				ft_strlen(expand->newline) + len);
			i += len;
		}
	}
}

char	*expand_line(char *line, char **envp, t_minishell *minishell)
{
	t_expand		expand;

	if (!line)
		return (NULL);
	ft_bzero(&expand, sizeof(t_expand));
	if (find_env_var(line, ft_strlen(line), &expand.quote) == -1)
		return (line);
	expand.count = count_total_char(line, ft_strlen(line), minishell);
	/*printf("count total = %d\n", expand.count);*/
	if (expand.count == 0)
	{
		free(line);
		return (NULL);
	}
	/*ft_printf_fd(1, "---------------EXPAND--------------\n");*/
	expand.newline = ft_calloc(expand.count + 1, sizeof(char));
	if (!expand.newline)
		error_malloc(line, NULL, minishell, "expand line");
	expand.quote = NO;
	update_line(line, minishell, &expand);
	free(line);
	expand.quote = NO;
	if (find_env_var(expand.newline,
			ft_strlen(expand.newline), &expand.quote) != -1)
		expand.newline = expand_line(expand.newline, envp, minishell);
	/*ft_printf_fd(1, "EXPAND LINE:\n**%s**\n", expand.newline);*/
	/*ft_printf_fd(1, "----------------------------------\n");*/
	return (expand.newline);
}
