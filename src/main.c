/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:07:23 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/08 22:01:59 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_pauline(t_minishell *minishell)
{
	t_token *token = minishell->token;

	while (token != NULL)
	{
		ft_printf_fd(2, "%s\n", token->value);
		token= token->next;
	}
}


t_token	*tmp_add(char *src, int type)
{
	t_token *token;
	
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(src);
	token->type = type;
	token->next = NULL;
	return (token);
	
}



int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;

	/*     token factice pour les tests     */
	t_token *token1 = tmp_add("wc", IS_CMD);
	t_token *token2 = tmp_add("echo", IS_BUILT_IN);
	t_token *token3 = tmp_add("|", PIPE);
	token1->next = token2;
	token2->next = token3;
	minishell.token = token1;
	print_pauline(&minishell);
	
	(void)argc;
	(void)argv;
	if (execute(&minishell, envp) == 1)
		return (1);

	return (0);

}
