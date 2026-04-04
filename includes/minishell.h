/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:04:25 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/04 11:41:46 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>


typedef struct s_env
{
    char    *username;
}               t_env;

/***********************************************************************/
/*                            SRC                                      */
/***********************************************************************/
int		main(void);
/********************************************************** struct env */
void    init_struct_env(t_env *env);
void	free_struct_env(t_env *env);
/**************************************************************** term */



#endif