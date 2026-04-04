/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:04:25 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/04 17:08:14 by pswirgie         ###   ########.fr       */
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

typedef enum e_token_type
{
    WORD,
    REDIRECTION,
    PIPE,
    EQUAL,
    IS_CMD,
    IS_BUILT_IN,
    IS_FILE
}   t_token_type;

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

typedef struct s_exec
{
    char    *line;
    int     error;
}               t_exec;


/***********************************************************************/
/*                            SRC                                      */
/***********************************************************************/
int		main(void);
/************************************************************* execute */
int     execute(t_env *env);
/************************************************************ built-in */

int     echo(t_exec *exec);

/********************************************************** struct env */
void    init_struct_env(t_env *env);
// void	free_struct_env(t_env *env);
/**************************************************************** term */
int term_raw_mode(struct termios *oldt, struct termios *newt);



#endif