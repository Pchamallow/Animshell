/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 13:12:46 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/06 15:18:01 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	no_cmd(t_parse *parse, t_element *type)
// {
// 	type->is_cmd = 0;
// 	ft_printf_fd(2, "command not found: %s\n", type->cmd);
// 	parse->error = 127;
// }


// void	init_args_execve(t_parse *parse, char **envp)
// {
// 	if (parse->in.is_file == 1 && parse->in.is_cmd == 1)
// 	{
// 		if (path_cmd(parse, &parse->in, envp) == 0)
// 			tabs_for_execve(parse, &parse->in);
// 		else
// 			parse->in.is_cmd = 0;
// 	}
// 	if (parse->out.is_file == 1 && parse->out.is_cmd == 1)
// 	{
// 		if (path_cmd(parse, &parse->out, envp) == 0)
// 			tabs_for_execve(parse, &parse->out);
// 		else
// 			parse->out.is_cmd = 0;
// 	}
// }
