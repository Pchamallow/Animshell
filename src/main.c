/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:07:23 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/10 13:41:41 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*tmp_add(char *src, int sort)
{
	t_token *token;
	
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(src);
	token->type = sort;
	token->next = NULL;
	return (token);
	
}



int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;

	/*     token factice pour les tests     */
	// t_token *token1 = tmp_add("cat", IS_CMD);
	t_token *token1 = tmp_add("wc", IS_CMD);
	// t_token *token1 = tmp_add("echo", IS_CMD);
	// t_token *token1 = tmp_add("echo", IS_BUILT_IN);
	
	t_token *token5 = tmp_add("<", IS_INPUT);
	t_token *token4 = tmp_add("infile.txt", IS_FILENAME);
	// t_token *token6 = tmp_add("coucou", IS_ARG);
	t_token *token6 = tmp_add("-l", IS_ARG);
	// t_token *token7 = tmp_add("lapin", IS_ARG);
	// t_token *token3 = tmp_add("|", PIPE);
	// t_token *token8 = tmp_add("wc", IS_CMD);
	// t_token *token9 = tmp_add("-l", IS_ARG);
	t_token *token10 = tmp_add(">", IS_OUTPUT);
	t_token *token11 = tmp_add("outfile.txt", IS_FILENAME);
	// t_token *token2 = tmp_add("echo", IS_BUILT_IN);
	token1->next = token5;
	token5->next = token4;
	token4->next = token6;
	token6->next = token10;
	token10->next = token11;
	// token6->next = token7;
	// token7->next = token3;
	// token3->next = token8;
	// token8->next = token9;
	// token9->next = token10;
	// token10->next = token11;
	minishell.token = token1;
	// print_pauline(&minishell);
	
	(void)argc;
	(void)argv;
	if (execute(&minishell, envp) == 1)
		return (minishell.exec.error);

	return (minishell.exec.error);

}
