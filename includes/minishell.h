/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:04:25 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/13 19:22:40 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <stdbool.h>
# include <sys/wait.h>

/***********************************************************************/
typedef enum e_token_type
{
	WORD,
	IS_CMD,
	IS_BUILT_IN,
	IS_ARG,
	IS_FILENAME,
	IS_DELIMITER,/* EOF */
	ONE_SPACE,
	PIPE,
	REDIRECTION,
	IS_INPUT,
	IS_OUTPUT,
	IS_APPEND,/* >> */
	HEREDOC /* << */
}			t_token_type;

typedef enum e_quote_type
{
	NO,
	SINGLE,
	DOUBLE
}			t_quote_type;

typedef struct s_token
{
	char			*value;
	char			*cmd_path;
	char			*path_explicite;
	char			**cmd_args;
	char			**args_execve;
	int				file_null;
	int				nb_opt;
	int				fd;
	// int				close;
	int				file_input; // savoir si c est un file type infile
	int				file_output;// savoir si c est un file type outfile
	t_token_type	type;
	t_quote_type	quote;
	struct s_token	*next;
}				t_token;

typedef enum e_built_in
{
	NONE,
	IS_ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT
}			t_built_in;

typedef enum e_put
{
	ERROR,
	TERMINAL,
	IS_FILE,
	IS_PIPE
}			t_put;

typedef struct s_pipe
{
	t_token *infile;
	t_token *outfile;
	t_token	*token;
	t_token	*cmd;
	int		is_cmd;
	int		nb_args;
	t_put	input;
	t_put	output;
	t_built_in	built_in;
	struct s_pipe	*next;
}				t_pipe;

typedef struct s_exec
{
	char		*line;
	char		*file_input;
	char		*file_output;
	int			error;
	int			input; // -1 file invalide, 0 pas de input, 1 = file, 2 = pipe
	int			output; // 0 pas de output(donc terminal), 1 = file, 2 = pipe
	int			index_pipe;
	int			nb_pipes;
	t_pipe		*pipe_lst;
	t_token		*last_pipe;
	t_token		**first_token;
}			t_exec;

typedef struct s_minishell
{
	t_exec		exec;
	t_token		*token;
}				t_minishell;

# define CMD_LIST "echo, cd, pwd, export, unset, env, exit"

/***********************************************************************/

/***********************************************************************/
/*                            SRC                                      */
/***********************************************************************/
int		main(int argc, char **argv, char **envp);
/************************************************************* execute */
int		execute(t_minishell *minishell, char **envp);
int		read_tokens(t_minishell *minishell, t_pipe *pipe, char **envp);
int		read_files(t_minishell *minishell, t_pipe *pipe, int pipes);
int		path_cmd(t_minishell *minishell, t_token *token, char **all_paths);
void	cmd_explicit(t_minishell *minishell, t_token *token);
char	*is_path(t_minishell *minishell, char **envp);
void	is_built_in(t_pipe *the_pipe, t_token *token);
void	print_pauline(t_minishell *minishell);
void	path_explicit(t_minishell *minishell, t_token *token);

/************************************************************ built-in */
int echo(t_minishell *minishell);
/***************************************************** tabs for execve */
void	init_args_execve(t_minishell *minishell, t_pipe *pipe);
/**************************************************** execute commands */
void	exec_cmds_pipe(t_minishell *minishell, char **envp);
void	exec_cmd_no_pipe(t_minishell *minishell, char **envp);

/********************************************************** struct env */
// void    init_struct_env(t_env *env);
// void	free_struct_env(t_env *env);
/********************************************************** read token */
void	init_cmd_args(t_minishell *minishell, t_pipe *pipe, int nb_args);
void	add_args(t_minishell *minishell, t_pipe *pipe, t_token *token);

/**************************************************************** term */
int		term_raw_mode(struct termios *oldt, struct termios *newt);
/********************************************************** error_free */
void	print_error_free(t_minishell *minishell, char *str, int error);
void	free_double(char **tab);
void	strerror_file(char *filename);
void	strerror_free_structure(t_minishell *minishell, char *filename, int error);
void	error_cmd_args(t_minishell *minishell, char *cmd, char *filename);
void	free_all(t_minishell *minishell);

/*************************************************************** utils */
int		len_double(char **tab);
int		len_cmd_no_endspace(char *str);
void	close_fds(t_minishell* minishell, t_pipe *pipe);
int		is_sign(char c);
/*************************************************************** TO_DELETE */
void print_double(char **str);// section to delete
bool find_built_in(char *token);

/************************************************************* parsing */
int		handle_quotes(char *line, t_token **token_list, int *index, char quote);
void	handle_pipe(char *line, t_token **token_list, int *index);
void	handle_redirection(char *line, t_token **token_list,
			int *index, char angle_bracket);
void	handle_words_no_quotes(char *line, t_token **token_list, int *index);
void	handle_spaces(char *line, t_token **token_list, int *index);
bool	is_whitespace(char c);
bool	is_separator(char c);
int		separate_into_tokens(char *line, t_token **token_list);
void	delete_next(t_token *token);
t_token	*case_heredoc(t_token *token, int *error);
t_token	*case_redirection(t_token *token, int *error);
t_token	*case_command(t_token *token, bool *cmd_found);
t_token	*case_arg(t_token *token);
t_token	*case_pipe(t_token *token, bool *cmd_found, int *error, t_token **head);
int		parse_tokens(t_token **token_list);
/********************************************************** token_list */
t_token	*ft_token_new(char *str, t_token_type token_type);
t_token	*ft_token_last(t_token *lst);
void	ft_token_add_back(t_token **head, t_token *newer, char *line);
void	ft_token_delone(t_token *lst, void (*del)(void *));
void	ft_token_lstclear(t_token **head);
/********************************************* tests print a supprimer */
void	print_tokens_types(t_token *token);// pour tester
void	print_tokens(t_token *token);// pour tester

#endif