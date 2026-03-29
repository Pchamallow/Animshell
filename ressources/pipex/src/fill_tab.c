/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_tab.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 16:47:01 by pswirgie          #+#    #+#             */
/*   Updated: 2026/03/27 10:34:29 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	cpy_tab_cmd(char *str, t_element *type)
{
	int	len;

	len = len_cmd(type->cmd) + 2;
	ft_strlcpy(str, type->cmd, len);
	return ;
}

static void	cpy_tab_content(char *str, t_element *type, int index)
{
	int	len;

	if (index == 0)
	{
		cpy_tab_cmd(str, type);
		return ;
	}
	index -= 1;
	if (type->options[index])
	{
		len = ft_strlen(type->options[index]) + 1;
		ft_strlcpy(str, type->options[index], len);
	}
}

void	fill_tab(char **tab, t_element *type)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		cpy_tab_content(tab[i], type, i);
		i++;
	}
}

static void	free_tab(t_parse *parse, char **tab, int len_tab)
{
	free_double(tab, len_tab - 1);
	print_error_free(parse, "Malloc failed.\n", EXIT_FAILURE);
}

char	**init_tab(t_element *type, t_parse *parse)
{
	char	**tab;
	int		len;
	int		len_tab;
	int		i;
	int		nb;

	len_tab = type->nb_opt + 2;
	i = 0;
	tab = malloc(sizeof(char *) * len_tab);
	if (!tab)
		print_error_free(parse, "Malloc failed.\n", EXIT_FAILURE);
	nb = len_tab - 1;
	len = ft_strlen(type->cmd) + 1;
	while (nb)
	{
		if (i > 0)
			len = ft_strlen(type->options[i - 1]) + 1;
		tab[i] = ft_calloc(len, sizeof(char));
		if (!tab[i])
			free_tab(parse, tab, len_tab);
		i++;
		nb--;
	}
	tab[i] = NULL;
	return (tab);
}
