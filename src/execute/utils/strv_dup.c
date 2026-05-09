/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strv_dup.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:29:46 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/09 15:05:53 by pswirgie         ###   ########.fr       */
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

int	strv_dup(t_minishell *minishell, char ***dst, char **src)
{
	int	len;
	int	i;

	i = 0;
	if (!src)
		return(-1);
	len = len_double(src) + 2;
	*dst = (char **)malloc((sizeof(char *)) * len);
	if (!*dst)
		print_error_free(minishell, "Error\nMalloc failed.\n", 1);
	len += 1;
	while (src[i])
	{
		(*dst)[i] = ft_strdup(src[i]);
		if (!(*dst)[i])
			print_error_free(minishell, "Error\nMalloc failed.\n", 1);;
		i++;
	}
	(*dst)[i] = NULL;
	return (0);
}

int	ft_joinstr(char **result, char *str, bool reverse_order)
{
	char	*tmp;
	
	tmp = ft_strdup(*result);
	if (!tmp)
		return (1);
	free(*result);
	if (reverse_order == false)
	{
		*result = ft_strjoin(tmp, str);
		if (!*result)
			return (1);
	}
	else
	{
		*result = ft_strjoin(str, tmp);
		if (!*result)
			return (1);
	}
	free(tmp);
	return (0);
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
