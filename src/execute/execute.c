/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 14:11:38 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/09 14:12:43 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
temporaire pour les tests de l execute,
a remplacer par la function de Steph
*/
static void tmp_free(t_minishell *minishell)
{
	t_token *tmp;

	tmp = minishell->token;
	// ft_printf_fd(2, "ici\n");
	while (minishell->token != NULL)
	{
		// ft_printf_fd(2, "la\n");
		tmp = minishell->token;
		minishell->token = minishell->token->next;
		if (tmp->cmd_args != NULL)
			free_double(tmp->cmd_args);
		free(tmp->value);
		if (tmp->cmd_path)
			free(tmp->cmd_path);
		free(tmp);
		// minishell->token = minishell->token->next;
	}
}

void handle_sigint(int sig)
{
    // struct termios oldt, newt;
	// t_token	*first_token;

	// first_token = NULL;
	(void)sig;
	write(1, "\nminishell$ ", 12); // réaffiche le prompt
}

/*
Put values 0 or NULL in order to reuse after
*/
void	init_exec(t_minishell *minishell)
{
	t_token *tmp;
	
	minishell->exec.file_input = NULL;
	minishell->exec.file_output = NULL;
	// minishell->exec.cmd->value = NULL;
	minishell->exec.input = 0;
	minishell->exec.output = 0;
	minishell->exec.built_in = 0;
	minishell->exec.nb_args = 0;
	minishell->exec.index_pipe = 0;
	minishell->exec.last_pipe = minishell->token;
	tmp = minishell->token;
	while (tmp != NULL)
	{
		tmp->cmd_path = NULL;
		tmp->cmd_args = NULL;
		tmp->input = 0;
		tmp->output = 0;
		tmp->close = 0;
		tmp = tmp->next;
	}
}

int execute(t_minishell *minishell, char **envp)
{
	init_exec(minishell);
	
	read_tokens(minishell, &minishell->exec.pipe_a,
		minishell->token, envp);
	ft_printf_fd(2, "------------------\n");
	read_tokens(minishell, &minishell->exec.pipe_b,
		minishell->token, envp);
	print_pauline(minishell);//TO DELETE
	// ft_printf_fd(2, "%sici\n", minishell->token->value);
		
	/*  BOUCLE WHILE  */
	// signal(SIGINT, handle_sigint);

	// char *line;
	
	// while (1)
	// {
	// 	line = readline("minishell$ ");
	// 	if (!line)
	// 		break;

	// 	if (*line)
	// 		add_history(line);

	// 	// // PARSING ICI :************************
	// 	// separate_into_tokens(line, &first_token);
	// 	// parse_tokens(&first_token);
	// 	// ft_token_lstclear(&first_token);
	// 	// // *************************************
	// 	// read(STDIN_FILENO, &c, 1);

	// 	// printf("Tu as tapé : %c\n", c);

	// 	free(line);
	// }

	/* 
	free l history une fois tout fini
	*/

	tmp_free(minishell);

	return (0);
}
