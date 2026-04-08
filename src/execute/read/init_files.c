/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:08:45 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/08 21:18:01 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_infile(t_minishell *minishell, t_token *token)
{
	token->fd = open(token->value, O_RDONLY);
	if (token->fd < 0)
	{
		token->close = 1;
		minishell->exec.error = 2;
		strerror_print(token->value); //NOTE -  print ici ou stocker pour print aprs echo [par exemple] ? 
	}
	if (access(token->value, R_OK) != 0)
	{
		free(minishell->exec.file_input);
		minishell->exec.input = -1;
		minishell->exec.error = 1;
		return (-1);
	}
	return (0);
}

// static void	init_outfile(t_parse *parse, t_element *type, char *str)
// {
// 	if (type->is_file == 1)
// 	{
// 		type->file = str;
// 		type->fd = open(type->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 		if (type->fd < 0)
// 		{
// 			type->close = 1;
// 			type->is_file = 0;
// 			strerror_print(type->file);
// 			parse->error = 2;
// 		}
// 		if (access(type->file, W_OK) != 0)
// 		{
// 			type->is_file = 0;
// 			parse->error = 1;
// 		}
// 	}
// }
