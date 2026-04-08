/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 16:07:17 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/08 22:08:14 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"




void	free_cpy(char *dst, char *src)
{
	if (dst != NULL)
		free(dst);
	dst = ft_strdup(src);
}


void	read_files(t_minishell *minishell, t_token *token)
{
	/*garder les erreurs de tests */
	/* permission OK, exit ou est creer pour un outfile*/
	if (token->input == 1)
	{
		if (init_infile(minishell, token) == 0)
			free_cpy(minishell->exec.file_input, token->value);
	}
	if (token->output == 1)
		free_cpy(minishell->exec.file_output, token->value);
}

/*
TODO verifier que ca fait bien toute la pipe
*/
int	find_pipe(t_token *token)
{
	int	i;

	i = 0;
	while (token != NULL && token->type != PIPE)
	{
		token = token->next;
		i++;
	}
	return (i);
}

/*
A. IS_CMD
path_cmd : 
1. is an environnement variable PATH ?
2. is a explicit path, absolute path ? 
Or path is to search
3. is cmd valid ?

B. IS_BUILT_IN
1. what built in is ? 


*/

		// /* PRINT ICI */
		// ft_printf_fd(2, "%s\n", token->cmd_path);

int read_tokens(t_minishell *minishell, t_token *token, char **envp)
{
	char	**all_paths;
	char	*paths_one_line;
	int		i;
	int		pipe;

	// print_tokens(token);
	i = 0;
	paths_one_line = is_path(minishell, envp);
	all_paths = ft_split(paths_one_line, ':');
	pipe = find_pipe(token);
	while (token != NULL && i <= pipe)
	{
		if (token->type == IS_CMD)
			path_cmd(minishell, token, all_paths);
		// else if (token->type == IS_BUILT_IN)
		// 	is_build_in(minishell, token);
		// else if (token->type == IS_ARG)
		else if (token->type == IS_FILENAME)
			read_files(minishell, token);
		else if (token->type == IS_INPUT && token->next != NULL)
			token->next->input = 1;
		else if (token->type == IS_OUTPUT && token->next != NULL)
			token->next->output = 1;
		token = token->next;
		i++;
	}
	// il y a un dernier infile, mais il y a une erreure
	// donc une erreure a print apres echo
	// if (minishell->exec.input == -1)
	free_double(all_paths);
	// ft_printf_fd(2, "ici\n");
	return (0);
}
