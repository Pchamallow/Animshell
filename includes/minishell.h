/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:04:25 by pswirgie          #+#    #+#             */
/*   Updated: 2026/06/02 12:04:40 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <stdbool.h>
# include <limits.h>
# include <dirent.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include "execute.h"
# include "parsing.h"

/***********************************************************************/
extern volatile sig_atomic_t	g_sig_value;

typedef struct s_minishell
{
	t_exec		exec;
	t_token		*token;
	t_token		*here_doc;
	t_builtin	builtin;
}				t_minishell;
/***********************************************************************/

#endif
