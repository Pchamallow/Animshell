/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 14:55:25 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/13 14:54:29 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
Find nb of pipes, 
fill 0 or NULL,
*/
void	init_pipe(t_minishell *minishell)
{
	t_pipe	*current;
	int	max;
	int	i;

	i = 0;
	max = nb_pipes(minishell->token);
	minishell->exec.nb_pipes = max;
	// printf("max = %d\n", max);
	minishell->exec.pipe_lst = ft_calloc(1, sizeof(t_pipe));
	if (!minishell->exec.pipe_lst)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	current = minishell->exec.pipe_lst;
	while (i <= max)
	{
		// printf("ICI\n");
		current->is_cmd = 0;
		current->nb_args = 0;
		current->error = 0;
		current->builtin_kind = NONE;
		current->input = TERMINAL;
		current->output = TERMINAL;
		current->infile = NULL;
		current->outfile = NULL;
		if (i < max)
		{
			current->next = ft_calloc(1, sizeof(t_pipe));
			if (!current->next)
				return ;
			current = current->next;
		}
		i++;
	}
}
