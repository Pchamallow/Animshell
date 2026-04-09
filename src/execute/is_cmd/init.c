/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 18:51:47 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/09 21:10:20 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int	index_first_opt(char *str)
// {
// 	int	i;

// 	i = ft_strlen(str);
// 	while (str[i] && str[i] == ' ')
// 		i++;
// 	return (i);
// }

// static int	index_next_opt(char *str, int i)
// {
// 	while (str[i] && ft_isalpha(str[i]) == 1)
// 		i++;
// 	while (str[i] && (ft_isalpha(str[i]) != 1) && is_sign(str[i]) != 1)
// 		i++;
// 	return (i);
// }

// /*
// for malloc = nb_opt + 1 for '\0'
// nb_options = 0 to max_option = 3 options = 0 to 2  -> nb_opt - 1
// */
// void	init_options(char *str, t_element *type, t_parse *parse)
// {
// 	int	nb_options;
// 	int	i;
// 	int	j;
// 	int	len;

// 	i = 0;
// 	j = 0;
// 	nb_options = type->nb_opt;
// 	type->options = malloc(sizeof(char *) * (type->nb_opt + 1));
// 	if (!type->options)
// 		print_error_free(parse, "Error\nMalloc options failed.\n", 1);
// 	i = index_first_opt(str);
// 	while (nb_options)
// 	{
// 		len = len_option(&str[i]) + 1;
// 		type->options[j] = ft_calloc(len, sizeof(char));
// 		if (!type->options[j])
// 			print_error_free(parse, "Error\nMalloc options failed.\n", 1);
// 		nb_options--;
// 		i = index_next_opt(str, i);
// 		j++;
// 	}
// 	type->options[j] = NULL;
// 	cpy_options(type->options, str);
// }

// /**
//  * all parse's elements at NULL or 0.
// */
// void	init_parse(t_parse *parse, t_element *type, int i)
// {
// 	type->file = NULL;
// 	type->file_null = 0;
// 	type->cmd = NULL;
// 	type->cmd_path = NULL;
// 	type->path_explicite = NULL;
// 	type->options = NULL;
// 	type->nb_opt = 0;
// 	type->fd = 0;
// 	type->args_execve = NULL;
// 	type->is_file = 1;
// 	type->is_cmd = 1;
// 	type->close = 0;
// 	if (i == 1)
// 		return ;
// 	parse->error = 0;
// 	init_parse(parse, &parse->out, 1);
// }
