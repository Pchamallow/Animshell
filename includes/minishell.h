/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:04:25 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/16 21:59:39 by stkloutz         ###   ########.fr       */
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

# define BLUE	"\033[34m"// a delete pour les commentaires
# define RESET	"\033[0m"// a delete. pour les commentaires
# define ERROR_MSG(msg) BLUE msg RESET //

/***********************************************************************/
extern volatile sig_atomic_t	g_sig_value;

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
	REDIRECTION, /* valeur intermediaire durant le parsing */
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

typedef enum e_builtin_kind
{
	NONE,
	IS_ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT
}			t_builtin_kind;

typedef enum e_put
{
	ERROR,
	TERMINAL,
	IS_FILE,
	IS_HEREDOC,
	IS_PIPE
}			t_put;

typedef struct	s_expand
{
	char			*newline;
	int				count;
	t_quote_type	quote;
}				t_expand;

typedef struct s_builtin_content
{
	char		*result;
	bool		for_prompt;
	// t_path_kind	path;
}				t_builtin_content;

typedef struct s_builtin
{
	t_builtin_content	echo;
	t_builtin_content	cd;
	t_builtin_content	pwd;
}				t_builtin;

typedef struct s_pipe
{
	t_token 		*infile;
	t_token 		*outfile;
	t_token			*token;
	t_token			*cmd;
	int				is_cmd;
	int				nb_args;
	int				error;
	int				pipfd1;
	t_put			input;
	t_put			output;
	// t_builtin		builtin;
	t_builtin_kind	builtin_kind;
	struct s_pipe	*next;
}				t_pipe;

typedef struct s_exec
{
	char		*line;
	char		*file_input;
	char		*file_output;
	char		**paths_for_search_cmd;
	char		**envp;
	int			error;
	int			input; // -1 file invalide, 0 pas de input, 1 = file, 2 = pipe
	int			output; // 0 pas de output(donc terminal), 1 = file, 2 = pipe
	int			index_pipe;
	int			index_prev_pipe;
	int			nb_pipes;
	t_pipe		*pipe_lst;
	t_token		*last_pipe;
	t_token		**first_token;
}			t_exec;

typedef struct s_minishell
{
	t_exec		exec;
	t_token		*token;
	t_token		*here_doc;
	t_builtin	builtin;
	char		*prompt;
}				t_minishell;

/***********************************************************************/

/***********************************************************************/
/*                            SRC                                      */
/***********************************************************************/
int		main(int argc, char **argv, char **envp);
/**************************************************************** init */
void	init_exec(t_minishell *minishell);
void	init_pipe(t_minishell *minishell);
/************************************************************* execute */
int		execute(t_minishell *minishell, char **envp);
void	get_paths_for_cmd(t_minishell *minishell);
int		read_tokens(t_minishell *minishell, t_pipe *pipe, int fd);
int		nb_args(t_token *token);
int		find_input_output(t_minishell *minishell, t_pipe *pipe, int fd);
int		path_cmd(t_minishell *minishell, t_token *token);
void	cmd_explicit(t_minishell *minishell, t_token *token);
// char	*is_path(t_minishell *minishell, char **envp);
void	is_built_in(t_pipe *the_pipe, t_token *token);
// int		heredoc(t_minishell *minishell, t_pipe *pipe, t_token *token);
int	heredoc(t_minishell *minishell, t_token *token, int fd);
int		nb_pipes(t_token *first);

/************************************************************ built-in */
int		cd(t_minishell *minishell, t_pipe *pipe);
int		echo(t_minishell *minishell, t_pipe *pipe);
void	echo_for_prompt(t_minishell *minishell, t_pipe *pipe);
int		env(t_minishell *minishell, t_pipe *pipe);
void	is_exit(t_minishell *minishell, t_pipe *pipe);
int		export_print(t_minishell *minishell, t_pipe *pipe);
int		export(t_minishell *minishell, t_pipe *pipe);
bool	is_concat(char *arg, int i);
int		concat_found(char *arg);
char	*new_concat(char *arg);
bool	is_valid_arg(char *arg);
int		found_var(char **envp, char *var);
int		ft_env_cmp(const char *s1, const char *s2);
int		get_envp_len(char **envp);
char	**envp_copy(char **envp, int len);
char	**update_envp(t_minishell *minishell, t_token *arg, int count);
bool	is_same_name(char *env_var, t_token *arg);
int		unset(t_minishell *minishell, t_pipe *pipe);
int		pwd(t_minishell *minishell, t_pipe *pipe);
void	init_pwd_envp(t_minishell *minishell);
void	init_pwd(t_minishell *minishell);
/***************************************************** tabs for execve */
void	init_args_execve(t_minishell *minishell, t_pipe *pipe);
/**************************************************** execute commands */
void	exec_cmds_pipe(t_minishell *minishell);

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
void	strerror_file(char *filename);
void	strerror_free_structure(t_minishell *minishell, char *filename, int error);
void	error_cmd_args(char *cmd, char *filename, char *error);
/**************************************************************** free */
void	lst_pipe_clear(t_pipe **head);
void	free_all(t_minishell *minishell);
void	free_strv(char **array);
void	free_heredoc(t_minishell *minishell);
void	free_envp(t_minishell *minishell);

/*************************************************************** utils */
int		len_double(char **tab);
int		len_cmd_no_endspace(char *str);
void	close_fd(int *fd);
void	close_fds_pipe(t_pipe *pipe);
int		is_sign(char c);
int		strv_dup(t_minishell *minishell, char ***dst, char **src);
int		lst_size(t_token *token);
int		count_chr(char *str, char c, bool followed);
bool	find_built_in(char *token);
int		ft_joinstr(char **result, char *str, bool reverse_order);
void	ft_joinchr(t_minishell *minishell, char **result, char c);
int		ft_strcmp(char *s1, char *s2);
void	ft_strcpy(char *dst, char *src);
char	*safe_join(char *s1, char *s2);
/************************************************************** utils_char */
char	*str_beginend_char(t_minishell *minishell, char *str, char c);
int		index_lastchar(char *str, char c);
int	remove_begin(char **str, char c, int start, int end);
// int		remove_begin(char **str, char c);
int		remove_end(char **str, char c);
int		join_oldnew(char **old, char **new);
/*************************************************************** utils_str */
int		strv_searchindex(char **strv, char *search);
int		is_double_quoted(char *str);
int		has_alpha(char *str);
int		str_copy_and_free(char **src, char **dst);
int		cpy_strvindex(char **result, char **src, char *search);
/************************************************************** utils_strv */
int		memcpy_strv(char **dst, char **src, int max);
// int		memcpy_strv2(char **dst, char **src, int max);
/*************************************************************** TO_DELETE */
void	print_double(char **str);// section to delete
void	print_pipefd(int fd1, int fd2);
void	print_pauline(t_minishell *minishell);
/****************************************************************** expand */
void	toggle_quote(char c, t_quote_type *quote);
int		find_env_var(char *line, int len, t_quote_type *quote);
int		get_var_name_len(char *line);
int		get_var(char *line, char **envp, int wd_len);
bool	quote_found(char *str);
int		count_total_char(char *line, int len, t_minishell *minishell);
void	ft_strlcat_add_quotes(char *dst, const char *src, size_t size);
char	*expand_line(char *line, char **envp, t_minishell *minishell);
/***************************************************************** parsing */
int		handle_quotes(char *line, t_token **token_list, int *index,
			t_minishell *minishell);
void	handle_pipe(char *line, t_token **token_list, int *index,
			t_minishell *minishell);
void	handle_redirection(char *line, int *index, char angle_bracket,
			t_minishell *minishell);
void	handle_words_no_quotes(char *line, t_token **token_list, int *index,
			t_minishell *minishell);
void	handle_spaces(char *line, t_token **token_list, int *index,
			t_minishell *minishell);
bool	is_whitespace(char c);
bool	is_separator(char c);
int		separate_into_tokens(char *line, t_token **token_list,
			t_minishell *minishell);
void	delete_next(t_token *token);
void	join_next_token(t_token *token, char *line, t_minishell *minishell);
t_token	*case_heredoc(t_token *token, int *error, char *line,
			t_minishell *minishell);
t_token	*case_redirection(t_token *token, int *error, char *line,
			t_minishell *minishell);
t_token	*case_command(t_token *token, bool *cmd_found, char *line,
			t_minishell *minishell);
t_token	*case_arg(t_token *token, char *line, t_minishell *minishell);
t_token	*case_pipe(t_token *token, bool *cmd_found, int *error, t_token **head);
int		parse_tokens(char *line, t_token **token_list, t_minishell *minishell);
/*******************************************************errors parsing */
void	free_line_and_token_list(char *line, t_token **token_list);
void	error_malloc(char *line, char *newline, t_minishell *minishell,
			char *err_msg);
void	error_quote(char *line, t_token **token_list, t_minishell *minishell);
/********************************************************** token_list */
t_token	*ft_token_new(char *str, t_token_type token_type);
t_token	*ft_token_last(t_token *lst);
void	ft_token_add_back(t_token **head, t_token *newer, char *line, t_minishell *minishell);
void	ft_token_delone(t_token *lst, void (*del)(void *));
void	ft_token_lstclear(t_token **head);
/********************************************************** signals */
void	set_signal_interactive(void);
void	set_signal_heredoc(void);
void	reset_signal_to_default(void);
void	ignore_signal(void);
void	check_signal_value(t_minishell *minishell);
int		check_signal_heredoc(char *str, int signal);
void	get_exit_status(t_minishell *minishell);
/********************************************* tests print a supprimer */
void	print_tokens_types(t_token *token);// pour tester
void	print_tokens(t_token *token);// pour tester

#endif
