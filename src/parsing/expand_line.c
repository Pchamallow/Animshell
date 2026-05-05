/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 11:21:18 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/04 22:57:43 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	toggle_quote(char c, t_quote_type *quote)
{
	if (c == '\"')
	{
		if (*quote == NO)
			*quote = DOUBLE;
		else if (*quote == DOUBLE)
			*quote = NO;
	}
	if (c == '\'')
	{
		if (*quote == NO)
			*quote = SINGLE;
		else if (*quote == SINGLE)
			*quote = NO;
	}
}

/*
** checks if '$' followed by a char different from '?'
*/
int	find_env_var(char *line, int len, t_quote_type *quote)
{
	int				i;

	i = 0;
	while (i < len - 1)
	{
		toggle_quote(line[i], quote);
		if (*quote != SINGLE && line[i] == '$' && line[i + 1] != '$'
				&& !is_separator(line[i + 1]))
			return (i);
		i++;
	}
	return (-1);
}

int	get_var_name_len(char *line)
{
	int	wd_len;
	int	len;
	int	i;

	wd_len = 0;
	i = 0;
	len = ft_strlen(line);
	while (i + wd_len < len && !is_separator(line[i + wd_len])
				&& line[i + wd_len] != '$')
		wd_len++;
	return (wd_len);
}

int	get_var(char *line, char **envp, int wd_len)
{
	int	j;

	if (!envp)
		return (-1);
	j = 0;
	while (envp[j] && ft_strncmp(line, envp[j], wd_len) != 0)
		j++;
	return (j);
}

int	get_exit_status_len(int exit_status)
{
	int	count;

	if (exit_status < 0 || exit_status > 255)
		exit_status = 255;
	count = 1;
	while (exit_status >= 10)
	{
		exit_status = exit_status / 10;
		count++;
	}
	return (count);
}

/*compte nb char*/
int	count_total_char(char *line, int len, char **envp, t_minishell *minishell)
{
	int				i;
	int				j;
	int				count;
	int				wd_len;
	t_quote_type	quote;

	count = len;
	i = 0;
	quote = NO;
	while (i < len)
	{
		toggle_quote(line[i], &quote);
		if (quote != SINGLE && line[i] == '$' && line[i + 1] == '?')
			count += get_exit_status_len(minishell->exec.error) - 2;
		else if (quote != SINGLE && line[i] == '$' && line[i + 1] != '$'
				&& line[i + 1] != '?' && !is_separator(line[i + 1]))
		{
			i++;
			wd_len = get_var_name_len(line + i);
			j = get_var(line + i, envp, wd_len);
			if (j != -1 && envp[j] != NULL)
				count += (ft_strlen(envp[j]) - (wd_len + 1));
			count -= (wd_len + 1);
		}
		i++;
	}
	return (count);
}

size_t	ft_strlcat_minishell(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	j = ft_strlen(src);
	if (dst)
		i = ft_strlen(dst);
	else
		i = 0;
	j = 0;
	while ((i + j < (size)) && src[j])
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = '\0';
	return (i + j);
}

/*boucle principale avec calloc + strlcopy / strlcat*/
char	*expand_line(char *line, char **envp, t_minishell *minishell)
{
	int				count;
	char			*newline;
	int				i;
	int				len;
	int				wd_len;
	int				j;
	t_quote_type	quote;
	char			*exit_str;

	if (!line)
		return (NULL);
	quote = NO;
	if (find_env_var(line, ft_strlen(line), &quote) == -1)
		return (line);
	/*ft_printf_fd(1, "---------------EXPAND--------------\n");*/
	count = count_total_char(line, ft_strlen(line), envp, minishell);
	/*ft_printf_fd(1, "count final: %d\n", count);*/
	if (count < 0)
	{
		ft_printf_fd(2, "Problem with count_total_char\n");
		return (line);
	}
	if (count == 0)
	{
		free(line);
		return (NULL);
	}
	newline = ft_calloc(count + 1, sizeof(char));
	if (!newline)
		error_malloc(line, "malloc error in minishell expand line");
	i = 0;
	quote = NO;
	exit_str = NULL;
	while (i < (int)ft_strlen(line))
	{
		len = find_env_var(line + i, ft_strlen(line + i), &quote);
		if (len == -1)
		{
			ft_strlcat_minishell(newline, line + i, count + 1);
			i += ft_strlen(line + i);
		}
		else if (len == 0 && line[i + 1] == '?')
		{
			if (!exit_str)
				exit_str = ft_itoa(minishell->exec.error);
			if (!exit_str)
				error_malloc(line, "malloc error in minishell expand line");
			ft_strlcat_minishell(newline, exit_str, count + 1);
			i += 2;
		}
		else if (len == 0 && line[i + 1] != '?')
		{
			wd_len = get_var_name_len(line + i + 1);
			j = get_var(line + i + 1, envp, wd_len);
			if (j != -1 && envp[j] != NULL)
				ft_strlcat_minishell(newline, envp[j] + wd_len + 1, count + 1);
			i += (wd_len + 1);
		}
		else
		{
			/*ft_printf_fd(1, "len = %d\n", len);*/
			ft_strlcat_minishell(newline, line + i, ft_strlen(newline) + len);
			i += len;
		}
	}
	if (exit_str)
		free(exit_str);
	free(line);
	//S'il y a des variables qui contiennent des variables, on refait un tour :
	quote = NO;
	if (find_env_var(newline, ft_strlen(newline), &quote) != -1)
		newline = expand_line(newline, envp, minishell);
	ft_printf_fd(1, "EXPAND LINE:\n**%s**\n", newline);
	/*ft_printf_fd(1, "----------------------------------\n");*/
	return (newline);
}
