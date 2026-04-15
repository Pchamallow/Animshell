/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 11:21:18 by stkloutz          #+#    #+#             */
/*   Updated: 2026/04/15 17:42:33 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*check si $ suivi de char != ? dans line*/
int	find_env_var(char *line, int len)
{
	int	i;

	if (!line)
		return (0);
	i = 0;
	while (i < len - 1)
	{
		if (line[i] == '$' && line[i + 1] != '$'
				&& line[i + 1] != '?' && !is_separator(line[i + 1]))
			return (i);
		i++;
	}

	return (0);
}

int	get_var_name(char *line)
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

	j = 0;
	while (envp[j] && ft_strncmp(line, envp[j], wd_len) != 0)
		j++;
	return (j);
}

/*compte nb char*/
int	count_total_char(char *line, int len, char **envp)
{
	int		i;
	int		j;
	int		count;
	int		wd_len;
	bool	single_quote;

	count = len;
	ft_printf_fd(1, "count avant la boucle: %d\n", count);
	i = 0;
	single_quote = false;
	while (i < len)
	{
		if (line[i] == '\'')
		{
			if (single_quote == false)
				single_quote = true;
			else
				single_quote = false;
		}
		if (line[i] == '$' && line[i + 1] != '$'
				&& line[i + 1] != '?' && !is_separator(line[i + 1]))
		{
			i++;
			//1.find word:
			/*while (i + wd_len < len && !is_separator(line[i + wd_len])*/
						/*&& line[i + wd_len] != '$')*/
				/*wd_len++;*/
			wd_len = get_var_name(line + i);
			ft_printf_fd(1, "wd_len: %d\n", wd_len);
			//find word in envp:
			/*j = 0;*/
			/*while (envp[j] && ft_strncmp(line + i, envp[j], wd_len) != 0)*/
				/*j++;*/
			//3.count char:
			j = get_var(line + i, envp, wd_len);
			if (envp[j] != NULL)
				count += (ft_strlen(envp[j]) - (wd_len + 1));
			/*ft_printf_fd(1, "$ENV: %s\n", envp[j]);*/
			/*ft_printf_fd(1, "count + $ENV: %d\n", count);*/
			count -= (wd_len + 1);
			/*ft_printf_fd(1, "count - wd_len: %d\n", count);*/
		}
		i++;
	}
	return (count);
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
	ft_printf_fd(1, "---------------COUNT--------------\n");
	if (!find_env_var(line, ft_strlen(line)))
	{
		ft_printf_fd(2, "pas trouvé de $ENV\n");
		return (line);
	}
	count = count_total_char(line, ft_strlen(line), envp);
	ft_printf_fd(1, "count final: %d\n", count);
	if (count < 0)
	{
		ft_printf_fd(2, "Problem with count_total_char\n");
		return (line);
	}
	newline = ft_calloc(count + 1, sizeof(char));
	if (!newline)
	{
		ft_printf_fd(1, "Error calloc\n");
		return (NULL);
	}
	ft_printf_fd(1, "calloc réussi\n");
	i = 0;
	while (line[i])
	{
		len = find_env_var(line + i, ft_strlen(line + i));
		if (len == 0 && line[i] != '$')
			i = ft_strlcat(newline, line + i, count + 1);
		else if (len == 0 && line[i] == '$')
		{
			wd_len = get_var_name(line + i);
			j = get_var(line + i, envp, wd_len);
			ft_strlcat(newline, envp[j] + wd_len + 1, count + 1);
			i += (wd_len + 1);
		}
		else
			i = ft_strlcat(newline, line + i, len);
	}
	free(line);
	ft_printf_fd(1, "NEWLINE:\n%s\n", newline);
	ft_printf_fd(1, "----------------------------------\n");
	return (newline);
}
