/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:08:45 by pswirgie          #+#    #+#             */
/*   Updated: 2026/03/27 17:39:03 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_infile(t_parse *parse, t_element *type, char *str)
{
	if (type->is_file == 1)
	{
		type->file = str;
		type->fd = open(type->file, O_RDONLY);
		if (type->fd < 0)
		{
			type->close = 1;
			type->is_file = 0;
			parse->error = 2;
			if (parse->out.is_file == 0 && type->file_null == 0)
				strerror_print(parse->in.file);
			else if (type->file_null == 0)
				strerror_print(parse->in.file);
		}
		if (access(type->file, R_OK) != 0)
		{
			type->is_file = 0;
			parse->error = 1;
		}
	}
}

static void	init_outfile(t_parse *parse, t_element *type, char *str)
{
	if (type->is_file == 1)
	{
		type->file = str;
		type->fd = open(type->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (type->fd < 0)
		{
			type->close = 1;
			type->is_file = 0;
			strerror_print(type->file);
			parse->error = 2;
		}
		if (access(type->file, W_OK) != 0)
		{
			type->is_file = 0;
			parse->error = 1;
		}
	}
}

void	init_files(t_parse *parse, char **argv)
{
	init_infile(parse, &parse->in, argv[1]);
	init_outfile(parse, &parse->out, argv[4]);
}
