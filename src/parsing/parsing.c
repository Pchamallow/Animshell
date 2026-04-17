/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 21:18:24 by stkloutz          #+#    #+#             */
/*   Updated: 2026/04/17 15:00:54 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_next(t_token *token)
{
	t_token	*tmp;

	tmp = token->next;
	token->next = token->next->next;
	ft_token_delone(tmp, free);
	return ;
}

/*	**********************************************		*/
/* parse_tokens and subfunctions:						*/
/*	1.define more precisely the type of the token:		*/
/*		- WORD becomes IS_CMD, IS_ARG or IS_FILENAME	*/
/*		- REDIRECTION becomes IS_ INPUT, IS_OUTPUT		*/
/*							  HEREDOC or IS_APPEND		*/
/*		- no change of type for PIPE and ONE_SPACE		*/
/*	2.remove unnecessary spaces:						*/
/*		Only spaces between 2 IS_ARG are left.			*/
/*	3.Check for syntax errors:							*/
/*		- REDIRECTIONS must be followed by IS_FILENAME	*/
/*		- PIPE must not be followed by another PIPE		*/
/*					not be the last token				*/
/*					not be the first token				*/
/*	**********************************************		*/
int	parse_tokens(char *line, t_token **token_list)
{
	t_token	*token;
	bool	cmd_found;
	int		error;

	token = *token_list;
	cmd_found = false;
	error = 0;
	while (!error && token)
	{
		if (!error && token && token->type == REDIRECTION)
			token = case_redirection(token, &error);
		if (!error && token && token->type == WORD && !cmd_found)
			token = case_command(token, &cmd_found);
		if (!error && token && token->type == WORD && cmd_found)
			token = case_arg(token);
		if (!error && token && token->type == PIPE)
			token = case_pipe(token, &cmd_found, &error, token_list);
	}
	// ft_printf_fd(1, "token list after parsing:\n");//pour test
	print_tokens_types(*token_list);//pour test
	if (error)
	{
		ft_token_lstclear(token_list);
		free(line);
	}
	return (error);
}
