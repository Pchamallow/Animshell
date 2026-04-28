/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strv_dup.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:29:46 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/28 16:48:48 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	len_s1;
	int	len_s2;
	int	result;

	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	if (len_s1 > len_s2)
		result = ft_strncmp(s1, s2, len_s1);
	else
		result = ft_strncmp(s1, s2, len_s2);
	return (result);
}

static int	search_path(t_minishell *minishell, char *str)
{
	if (ft_strnstr((const char *)str, "PATH=", 5) != NULL)
	{
		minishell->exec.paths_for_search_cmd = ft_split(str, ':');
		return (1);
	}
	return (0);
}

int	strv_dup(t_minishell *minishell, char ***dst, char **src)
{
	int	len;
	int	i;
	int is_path;

	i = 0;
	is_path = 0;
	if (!src)
		return(-1);
	len = len_double(src) + 2;
	*dst = (char **)malloc((sizeof(char *)) * len);
	if (!*dst)
		print_error_free(minishell, "Error\nMalloc failed.\n", 1);
	len += 1;
	while (src[i])
	{
		if (!is_path)
			is_path += search_path(minishell, src[i]);
		(*dst)[i] = ft_strdup(src[i]);
		if (!(*dst)[i])
			print_error_free(minishell, "Error\nMalloc failed.\n", 1);;
		i++;
	}
	(*dst)[i] = NULL;
	if (!is_path)
	{
		minishell->exec.paths_for_search_cmd = NULL;
		print_error_free(minishell, "PATH not found.\n", 2);
	}
	return (0);
}

void	ft_joinstr(t_minishell *minishell, char **result, char *str)
{
	char	*tmp;
	
	tmp = ft_strdup(*result);
	if (!tmp)
	{
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		return ;
	}
	free(*result);
	*result = ft_strjoin(tmp, str);
	if (!*result)
	{
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		return ;
	}
	free(tmp);
}

void	ft_joinchr(t_minishell *minishell, char **result, char c)
{
	char	*tmp;
	char	str[2];

	str[0] = c;
	str[1] = '\0';
	tmp = ft_strdup(*result);
	if (!tmp)
	{
		free(tmp);
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		return ;
	}
	free(*result);
	*result = ft_strjoin(tmp, str);
	if (!*result)
	{
		free(*result);
		free(tmp);
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		return ;
	}
	free(tmp);
}
