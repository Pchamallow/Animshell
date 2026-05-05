/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:27:22 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/05 16:47:02 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*str_beginend_char(t_minishell *minishell, char *str, char c)
{
	char	*new;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str) + 3;
	new = ft_calloc(len, sizeof(char));
	if (!new)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	new[0] = c;
	ft_strlcpy(&new[1], str, len);
	new[len - 2] = c;
	return (new);
}

int	lst_size(t_token *token)
{
	int	count;

	count = 0;
	if (!token)
		return (0);
	while (token)
	{
		count++;
		token = token->next;
	}
	return (count);
}

void	close_fds_pipe(t_pipe *pipe)
{
	if (pipe->infile && pipe->infile->fd >= 0)
		close(pipe->infile->fd);
	// else
		// printf("--------------- already closed infile\n");
	if (pipe->outfile && pipe->outfile->fd >= 0)
	{
		// printf("--------------- close outfile\n");
		close(pipe->outfile->fd);
	}
	// else
		// printf("---------- already closed infile\n");
}

void	close_fd(int fd)
{
	if (fd && fd >= 0)
		close(fd);
}

int	is_double_quoted(char *str)
{
	int	i;
	int	doubled;

	i = 0;
	doubled = 0;
	while (str[i])
	{
		if (str[i] == '"')
			doubled++;
		i++;
	}
	if (str[i] == '\0' && doubled == 2)
		return (1);
	return (0);
}

int	len_double(char **tab)
{
	int	i;

	i = 0;
	while (tab[i] != NULL)
		i++;
	if (i != 0)
		i--;
	return (i);
}

int	len_cmd_no_endspace(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ')
		{
			while (str[i] && str[i] != ' ')
				i++;
			break ;
		}
		i++;
	}
	return (i);
}

int	is_sign(char c)
{
	if (c == '<' || c == '>' || c == '|' || c == '&' || c == '-')
		return (1);
	return (0);
}

int	is_space(char *str, int not)
{
	int	i;

	i = 0;
	if (not == 1)
	{
		while (str[i] && str[i] != ' ')
			i++;
	}
	else
	{
		while (str[i] && str[i] == ' ')
			i++;
	}
	return (i);
}

bool find_built_in(char *token)
{
	const char *builtins[7];
	int	i;
	int	len_token;
	int	len_builtin;

	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	i = 0;
	len_token = ft_strlen(token);
	// len == le plus grand entre le token et le builtins
	while (i < 7)
	{
		len_builtin = ft_strlen(builtins[i]);
		if (len_token > len_builtin)
		{
			if (ft_strncmp(token, builtins[i], len_token) == 0)
				return (true);
		}
		else
		{
			if (ft_strncmp(token, builtins[i], len_builtin) == 0)
				return (true);
		}
		i++;
	}
	return (false);
}

void free_cpy(char **dst, char *src)
{
	if (*dst != NULL)
		free(*dst);
	*dst = ft_strdup(src);
}
