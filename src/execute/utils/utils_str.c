/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 11:28:00 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/08 11:16:30 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	*replace_str_by_chr(char *path, char *to_replace, char c)
// {
// 	char	*tmp;
// 	char	*new;
// 	// int	i;
// 	int		here;

// 	//remove // s il y en a un a la fin
// 	(void)c;
// 	tmp = ft_strnstr(path, to_replace, ft_strlen(path));
// 	here = tmp - path - 2;
// 	new = NULL;
// 	if (here == 0)
// 		new = ft_strdup(path);
// 		//ne pas suppr le double //
// 	printf("here = %d\n", here);
// 		// dans path[i] chercher la string to_replace
// 		// avoir un index au moins
// 		// lorsqu elle est trouver, remplacer par c 
// 		// rappeler la fonction jusqu' a que toutes la string soit clean
// 	return (tmp);
// }

int	cpy_strv(char ***dst, char **src, int max)
{
	int	i;

	i = 0;
	if (!*dst || !**dst || !src || !*src)
		return (1);
	while ((src[i] && max == 0) || (i < max))
	{
		(*dst)[i] = src[i];
		i++;
	}
	return (0);
}

int	cpy_strvindex(char **result, char **src, char *search)
{
	int	index;
	int	len_search;

	len_search = ft_strlen(search);
	index = strv_searchindex(src, search);
	if (index == -1)
		return (-1);
	*result = ft_substr(src[index], len_search, ft_strlen(src[index]) - len_search);
	if (!result)
		return (1);
	return (0);
}

int	strv_searchindex(char **strv, char *search)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(search);
	while (strv[i])
	{
		if (ft_strnstr(strv[i], search, len) != NULL)
			return (i);
		i++;
	}
	return (-1);
}

int	str_copy_and_free(char **src, char **dst)
{
	char	*tmp;

	tmp = ft_strdup(*src);
	if (!tmp)
		return (1);
	free(*dst);
	*dst = tmp;
	free(*src);
	*src = NULL;
	return (0);
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

int	has_alpha(char *str)
{
	int	alpha;
	int	end;
	// int	i;

	// i = 0;
	alpha = 0;
	end = ft_strlen(str) - 1;
	if (end <= 0)
		return (0);
	while (str[end] && end > 0)
	{
		if (ft_isalpha(str[end]))
		{
			alpha = 1;
			break;
		}
		end--;
	}
	return (alpha);
}
