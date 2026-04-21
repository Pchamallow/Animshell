/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:42:57 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/21 13:40:10 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../lib/libft/libft.h"
# include "../src/get_next_line/get_next_line.h"
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_token
{
	char				*value;
	char				*cmd_path;
	char				*path_explicite;
	char				**options;
	char				**args_execve;
	int					file_null;
	int					nb_opt;
	int					fd;
	int					close;
	t_token_type		type;
	t_token				*next;
}				t_token;

typedef struct s_parse
{
	t_element	in;
	t_element	out;
	int			error;
}		t_parse;

/***********************************************************************/
/*                            SRC                                      */
/***********************************************************************/
int		main(int argc, char **argv, char **envp);
/************************************************************* Parsing */
void	parsing(t_parse *parse, char **argv);
void	init_parse(t_parse *parse, t_element *type, int i);
void	init_files(t_parse *parse, char **argv);
void	init_infile(t_parse *parse, t_element *type, char *str);
/************************************************* Command and options */
void	init_cmd_and_options(int options, char *arg,
			t_element *type, t_parse *parse);
void	init_options(char *str, t_element *type, t_parse *parse);
/**********************  Command Tab for execve = (cmd, options, NULL) */
void	init_args_execve(t_parse *parse, char **envp);
char	**init_tab(t_element *type, t_parse *parse);
void	tabs_for_execve(t_parse *parse, t_element *type);
void	fill_tab(char **tab, t_element *type);
int		path_type(t_parse *parse, t_element *type);
void	path_explicit(t_parse *parse, t_element *type);
void	exec_cmds(t_parse *parse, char **envp);
/********************************************************* Errors_free */
void	free_double(char **tab, int len);
void	free_structure(t_element *type, t_parse *parse, int i);
void	strerror_free_structure(t_parse *parse, char *filename, int error);
void	strerror_print(char *filename);
void	print_error_free(t_parse *parse, char *str, int error);
void	print_error(char *str, int error);
/*************************************************************** Utils */
void	close_fds_pipe(t_parse *parse);
int		len_cmd(char *str);
int		len_cmd_no_endspace(char *str);
int		len_double(char **tab);
int		is_sign(char c);
int		is_option(char *str);
int		len_option(char *src);
void	cpy_options(char **options, char *src);
int		is_space(char *str, int not);
/***********************************************************************/
/*                            GNL                                      */
/***********************************************************************/
char	*get_next_line(int fd);
char	*ft_leftover(char *line);
char	*ft_content_line(int fd, char *storage, char *buff);
char	*find_char(char *s, char c);
int		end_gnl(char *line, int fd, int end);
int		end_file(char *file);

#endif