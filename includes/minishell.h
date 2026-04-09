/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:04:25 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/09 10:20:30 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <signal.h>
# include <stdbool.h>

/***********************************************************************/
typedef enum e_token_type
{
	WORD,
	IS_CMD,
	IS_BUILT_IN,
	IS_ARG,
	IS_FILENAME,
	IS_DELIMITER,
	ONE_SPACE,
	PIPE,
	REDIRECTION,
	IS_INPUT,
	IS_OUTPUT,
	IS_APPEND,
	HEREDOC
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
	char			**options;
	char			**args_execve;
	int				file_null;
	int				nb_opt;
	int				fd;
	int				close;
	t_token_type	type;
	t_quote_type	quote;
	struct s_token	*next;
}				t_token;

typedef struct s_exec
{
	char	*line;
	int		error;
}			t_exec;

typedef struct s_minishell
{
	t_exec		exec;
	t_token		token;
}				t_minishell;

# define CMD_LIST "echo, cd, pwd, export, unset, env, exit"

/***********************************************************************/

/***********************************************************************/
/*                            SRC                                      */
/***********************************************************************/
int		main(void);
/************************************************************* execute */
int		execute(t_minishell *minishell);
int		is_cmd(t_minishell *minishell);
int		path_cmd(t_minishell *minishell, t_token *token, char **all_paths);
void	free_double(char **tab, int len);

/************************************************************ built-in */

int     echo(t_exec *exec);

/********************************************************** struct env */
// void    init_struct_env(t_env *env);
// void	free_struct_env(t_env *env);
/**************************************************************** term */
int term_raw_mode(struct termios *oldt, struct termios *newt);
/********************************************************** error_free */
void	print_error_free(t_minishell *minishell, char *str, int error);
/*************************************************************** utils */
int	len_double(char **tab);
int	len_cmd_no_endspace(char *str);

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
