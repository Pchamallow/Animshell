/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 11:21:18 by stkloutz          #+#    #+#             */
/*   Updated: 2026/04/23 22:55:12 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*check si $ suivi de char != ? dans line*/
int	find_env_var(char *line, int len)
{
	int				i;
	t_quote_type	quote;

	/*if (!line)*/
		/*return (-1);*/
	i = 0;
	quote = NO;
	while (i < len - 1)
	{
		/*---------- toggle quote -----------*/
		if (line[i] == '\"')
		{
			if (quote == NO)
				quote = DOUBLE;
			else if (quote == DOUBLE)
				quote = NO;
		}
		if (line[i] == '\'')
		{
			if (quote == NO)
				quote = SINGLE;
			else if (quote == SINGLE)
				quote = NO;
		}
		/*----------------------------------*/
		if (quote != SINGLE && line[i] == '$' && line[i + 1] != '$'
				&& line[i + 1] != '?' && !is_separator(line[i + 1]))
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

/*compte nb char*/
int	count_total_char(char *line, int len, char **envp)
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
		/*---------- toggle quote -----------*/
		if (line[i] == '\"')
		{
			if (quote == NO)
				quote = DOUBLE;
			else if (quote == DOUBLE)
				quote = NO;
		}
		if (line[i] == '\'')
		{
			if (quote == NO)
				quote = SINGLE;
			else if (quote == SINGLE)
				quote = NO;
		}
		/*----------------------------------*/
		if (quote != SINGLE && line[i] == '$' && line[i + 1] != '$'
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
char	*expand_line(char *line, char **envp)
{
	int		count;
	char	*newline;
	int		i;
	int		len;
	int		wd_len;
	int		j;

	if (!line)
		return (NULL);
	if (find_env_var(line, ft_strlen(line)) == -1)
		return (line);
	/*ft_printf_fd(1, "---------------EXPAND--------------\n");*/
	count = count_total_char(line, ft_strlen(line), envp);
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
	while (i < (int)ft_strlen(line))
	{
		len = find_env_var(line + i, ft_strlen(line + i));
		if (len == -1)
		{
			ft_strlcat_minishell(newline, line + i, count + 1);
			i += ft_strlen(line + i);
		}
		else if (len == 0)
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
	free(line);
	//S'il y a des variables qui contiennent des variables, on refait un tour :
	if (find_env_var(newline, ft_strlen(newline)) != -1)
		newline = expand_line(newline, envp);
	/*ft_printf_fd(1, "EXPAND LINE:\n**%s**\n", newline);*/
	/*ft_printf_fd(1, "----------------------------------\n");*/
	return (newline);
}
