/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:42:48 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/09 21:07:34 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	len_cmd_no_endspace(char *str)
{
	int	i;
	int	cmd;

	i = 0;
	cmd = 0;
	while (str[i])
	{
		if (str[i] != ' ')
		{
			cmd = 1;
			while (str[i] && str[i] != ' ')
				i++;
			break ;
		}
		i++;
	}
	return (i);
}

void	strerror_print(char *filename)
{
	char	*err;

	err = strerror(errno);
	write(2, err, strlen(err));
	write(2, ": ", 2);
	write(2, filename, strlen(filename));
	write(2, "\n", 1);
}

static void	is_empty(t_parse *parse, char **argv)
{
	if (argv[1][0] == '\0')
	{
		parse->in.file_null = 1;
		parse->in.is_file = 1;
		ft_printf_fd(2, "No such file or directory:\n");
		parse->error = 2;
	}
	if (argv[2][0] == '\0')
		parse->in.is_cmd = 0;
	if (argv[3][0] == '\0')
		parse->out.is_cmd = 0;
	if (argv[4][0] == '\0')
	{
		parse->out.is_file = 0;
		init_infile(parse, &parse->in, argv[1]);
		if (parse->in.close == 0)
			close(parse->in.fd);
		ft_printf_fd(2, "No such file or directory:\n");
		parse->error = 1;
	}
	if (argv[1][0] == '\0' && argv[2][0] == '\0'
		&& argv[3][0] == '\0' && argv[4][0] == '\0')
		exit(1);
}

void	parsing(t_parse *parse, char **argv)
{
	char	*infile_cmd;
	char	*outfile_cmd;

	init_parse(parse, &parse->in, 0);
	is_empty(parse, argv);
	infile_cmd = argv[2];
	outfile_cmd = argv[3];
	init_files(parse, argv);
	if (parse->in.is_file == 1 && parse->in.is_cmd == 1)
	{
		parse->in.nb_opt = is_option(infile_cmd);
		init_cmd_and_options(parse->in.nb_opt, infile_cmd, &parse->in, parse);
	}
	if (parse->out.is_file == 1 && parse->out.is_cmd == 1)
	{
		parse->out.nb_opt = is_option(outfile_cmd);
		init_cmd_and_options(parse->out.nb_opt,
			outfile_cmd, &parse->out, parse);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_parse	parse;

	if (argc != 5)
		print_error("Error\nNot 4 arguments.\n", 2);
	parsing(&parse, argv);
	init_args_execve(&parse);
	exec_cmds(&parse, envp);
	free_structure(&parse.in, &parse, 0);
	return (parse.error);
}
