/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 16:33:12 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/27 17:31:24 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"
# include "execute.h"

/***********************************************************************/
typedef struct s_minishell	t_minishell;

typedef enum e_quote_type
{
	NO,
	SINGLE,
	DOUBLE
}			t_quote_type;

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
	int				file_input;
	int				file_output;
	t_token_type	type;
	t_quote_type	quote;
	struct s_token	*next;
}				t_token;

typedef struct s_expand
{
	char			*newline;
	int				count;
	t_quote_type	quote;
}				t_expand;

/***********************************************************************/
/*                           PARSING                                   */
/***********************************************************************/
/* expand **************************************************************/
void	toggle_quote(char c, t_quote_type *quote);
int		find_env_var(char *line, int len, t_quote_type *quote);
int		get_var_name_len(char *line);
int		get_var(char *line, char **envp, int wd_len);
bool	quote_found(char *str);
int		count_total_char(char *line, int len, t_minishell *minishell);
void	ft_strlcat_add_quotes(char *dst, const char *src, size_t size);
char	*expand_line(char *line, char **envp, t_minishell *minishell);
/* parsing *************************************************************/
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
bool	parsing_syntax_ok(char *line, t_minishell *minishell);
bool	find_built_in(char *token);
/* errors parsing ******************************************************/
void	free_line_and_token_list(char *line, t_token **token_list);
void	error_malloc(char *line, char *newline, t_minishell *minishell,
			char *err_msg);
void	error_quote(char *line, t_token **token_list, t_minishell *minishell);
void	print_error_unexpected_token(t_token *token);
/* token_list **********************************************************/
t_token	*ft_token_new(char *str, t_token_type token_type);
t_token	*ft_token_last(t_token *lst);
void	ft_token_add_back(t_token **head, t_token *newer,
			char *line, t_minishell *minishell);
void	ft_token_delone(t_token *lst, void (*del)(void *));
void	ft_token_lstclear(t_token **head);
/* signals *************************************************************/
void	set_signal_interactive(void);
void	set_signal_heredoc(void);
void	reset_signal_to_default(void);
void	ignore_signal(void);
void	check_signal_value(t_minishell *minishell);
int		check_signal_heredoc(char *str, int signal);
void	get_exit_status(t_minishell *minishell, pid_t last_pid);
void	get_exit_heredoc(t_minishell *minishell, int return_value, int pid);

#endif