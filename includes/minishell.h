/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:04:25 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/05 19:29:29 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
//ajout steph:
# include <stdbool.h>


typedef struct s_env
{
    char    *username;
}               t_env;

/***********************************************************************/
//ajout steph pour parsing :
typedef enum e_token_type
{
	WORD,
	REDIRECTION,
	PIPE,
	ONE_SPACE,
	EQUAL,
	IS_CMD,
	IS_BUILT_IN,
	IS_FILE
}			t_token_type;

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
	struct s_token	*next;
}				t_token;

/***********************************************************************/

/***********************************************************************/
/*                            SRC                                      */
/***********************************************************************/
int		main(void);
/************************************************************* execute */
int execute(t_env *env);

/********************************************************** struct env */
void    init_struct_env(t_env *env);
// void	free_struct_env(t_env *env);
/**************************************************************** term */
int term_raw_mode(struct termios *oldt, struct termios *newt);

/************************************************************* parsing */
void	handle_quotes(char *line, t_token **token_list, int *index, char quote);
void	handle_pipe(char *line, t_token **token_list, int *index);
void	handle_redirection(char *line, t_token **token_list,
			int *index, char angle_bracket);
void	handle_words_no_quotes(char *line, t_token **token_list, int *index);
void	handle_spaces(char *line, t_token **token_list, int *index);
bool	is_whitespace(char c);
bool	is_separator(char c);
int		separate_into_tokens(char *line, t_token **token_list);
/********************************************************** token_list */
t_token	*ft_token_new(char *str, t_token_type token_type);
t_token	*ft_token_last(t_token *lst);
void	ft_token_add_back(t_token **head, t_token *newer, char *line);
void	ft_token_delone(t_token *lst, void (*del)(void *));
void	ft_token_lstclear(t_token **head);

#endif
