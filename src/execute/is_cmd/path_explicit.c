/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_explicit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 15:11:20 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/04 16:13:56 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// static void	cmd_explicit(t_parse *parse, t_element *type)
// {
// 	int		len;

// 	len = ft_strlen(type->cmd_path) - 2;
// 	free(type->cmd);
// 	type->cmd = ft_calloc(sizeof(char *), len + 1);
// 	if (!type->cmd)
// 		print_error_free(parse, "Malloc failed.\n", EXIT_FAILURE);
// 	ft_strlcpy(type->cmd, &type->cmd_path[2], len + 1);
// }

// void	path_explicit(t_parse *parse, t_element *type)
// {
// 	int	len;

// 	len = ft_strlen(type->cmd);
// 	type->cmd_path = ft_calloc(sizeof(char *), len + 1);
// 	if (!type->cmd_path)
// 		print_error_free(parse, "Malloc failed.\n", EXIT_FAILURE);
// 	ft_strlcpy(type->cmd_path, type->cmd, len + 1);
// 	cmd_explicit(parse, type);
// }


