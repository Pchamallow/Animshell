/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 16:39:35 by pswirgie          #+#    #+#             */
/*   Updated: 2026/06/06 16:46:15 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "minishell.h"
# include "parsing.h"

/***********************************************************************/
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

typedef struct s_builtin_content
{
	char		*result;
	int			error;
}				t_builtin_content;

typedef struct s_builtin
{
	t_builtin_content	echo;
	t_builtin_content	cd;
	t_builtin_content	pwd;
}				t_builtin;

typedef struct s_pipe
{
	t_token			*infile;
	t_token			*outfile;
	t_token			*token;
	t_token			*cmd;
	int				is_cmd;
	int				nb_args;
	int				error;
	int				infile_error;
	int				outfile_error;
	t_put			input;
	t_put			output;
	t_builtin_kind	builtin_kind;
	struct s_pipe	*next;
}				t_pipe;

typedef struct s_exec
{
	char		**paths_for_search_cmd;
	char		**envp;
	long long	error;
	long long	error_sig;
	long long	error_last_child;
	long long	error_last_prompt;
	int			index_pipe;
	int			index_prev_pipe;
	int			nb_pipes;
	int			input_fd;
	int			pipe_actual;
	int			last_pid;
	t_pipe		*pipe_lst;
	t_token		*last_pipe;
	t_token		**first_token;
}			t_exec;

/***********************************************************************/
/*                            SRC                                      */
/***********************************************************************/
/* init ****************************************************************/
void	init_exec(t_minishell *minishell);
void	init_pipe(t_minishell *minishell);
void	cpy_tab_cmd(char *str, t_pipe *pipe);
int		is_cmd(t_token *words);
bool	is_pipe(t_pipe *pipe, t_token *token);
/* execute *************************************************************/
void	execute(t_minishell *minishell);
void	get_paths_for_cmd(t_minishell *minishell);
int		read_tokens(t_minishell *minishell, t_pipe *pipe, int fd);
int		nb_args(t_token *token);
int		init_cmd(t_minishell *minishell, t_pipe *pipe);
int		find_input_output(t_minishell *minishell, t_pipe *pipe, int fd);
int		is_directory(t_minishell *minishell, t_pipe *pipe, char *str);
int		path_cmd(t_minishell *minishell, t_pipe *pipe, t_token *token);
void	path_explicit(t_minishell *minishell, t_token *token);
void	cmd_explicit(t_minishell *minishell, t_token *token);
int		heredoc(t_minishell *minishell, t_token *token, int fd);
int		nb_pipes(t_token *first);
void	exec_child(t_minishell *minishell, t_pipe *current, int *pipefd);
void	free_garbage(t_minishell *minishell, t_pipe *current);
int		build_pipeline_structure(t_minishell *minishell,
			t_pipe *current, int *pipefd);
/* built-in ************************************************************/
void	is_perm_folder(t_minishell *minishell, t_builtin_content *cd);
int		root_with_folder(t_minishell *minishell);
int		is_pwd_invalid(void);
void	error_getcwd(t_minishell *minishell, t_pipe *pipe);
int		check_args(t_minishell *minishell, t_pipe *pipe);
void	replace_oldpwd(t_minishell *minishell, t_pipe *pipe);
void	modify_pwd_in_envp(t_minishell *minishell);
int		cd(t_minishell *minishell, t_pipe *pipe);
int		remove_lastfolder(char **new_path, int last_slash);
int		dir_permission(t_minishell *minishell, t_builtin_content *cd,
			char *original);
int		root(t_minishell *minishell, char **home);
void	echo(t_pipe *pipe);
int		env(t_minishell *minishell, t_pipe *pipe);
void	is_exit(t_minishell *minishell, t_pipe *pipe);
int		exit_single_arg(t_minishell *minishell, char *nb);
int		is_num_single_sign(char *str);
int		export_print(t_minishell *minishell, t_pipe *pipe);
int		ft_export(t_minishell *minishell, t_pipe *pipe);
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
int		pwd_print(t_minishell *minishell);
void	pwd_update(t_minishell *minishell);
void	init_pwd_envp(t_minishell *minishell);
void	pwd_init(t_minishell *minishell);
char	*init_oldpwd(t_minishell *minishell);
void	init_args_execve(t_minishell *minishell, t_pipe *pipe);
void	exec_cmds_pipe(t_minishell *minishell);
/* read token **********************************************************/
void	init_cmd_args(t_minishell *minishell, t_pipe *pipe, int nb_args);
void	add_args(t_minishell *minishell, t_pipe *pipe, t_token *token);
/* error_free **********************************************************/
void	strerror_free_structure(t_minishell *minishell, char *filename,
			int error);
void	error_cmd_args(char *cmd, char *filename, char *error);
void	strerror_file(char *filename);
void	print_error_free(t_minishell *minishell, char *str, int error);
/* free ****************************************************************/
void	free_all(t_minishell *minishell);
void	lst_pipe_clear(t_pipe **head);
void	free_heredoc(t_minishell *minishell);
void	free_envp(t_minishell *minishell);
/* utils ***************************************************************/
void	close_fds_pipe(t_pipe *pipe);
void	close_fd(int *fd);
int		len_cmd_no_endspace(char *str);
int		lst_size(t_token *token);
void	ft_strcpy(char *dst, char *src);
char	*safe_join(char *s1, char *s2);
/* utils_char **********************************************************/
int		count_chr(char *str, char c, bool followed);
int		index_lastchar(char *str, char c);
/* utils_str ***********************************************************/
int		ft_strcmp(char *s1, char *s2);
int		cpy_strvindex(char **result, char **src, char *search);
int		strv_searchindex(char **strv, char *search);
int		str_copy_and_free(char **src, char **dst);
int		has_alpha(char *str);
int		strfind_occurences(char *src, char *target, int index_of_occurrence);
int		strfind_last(char *src, char *target);
int		strfind(char *src, char *target, bool end);
int		str_iswhitespaces(char *s);
char	*join_oldnew(char *old, char *new, bool invert_order);
/* utils_strv **********************************************************/
void	free_strv(char **array);
int		memcpy_strv(char **dst, char **src, int max);
int		strvlen(char **array);
void	free_strv_len(char **array, int len);
char	**strv_dup(t_minishell *minishell, char **src);

#endif
