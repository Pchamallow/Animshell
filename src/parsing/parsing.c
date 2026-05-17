/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 21:18:24 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/16 21:54:33 by stkloutz         ###   ########.fr       */
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

/*	***********************************************		*/
/*join_next_token:										*/
/* - joins the value of the next token					*/
/*   to the current token value,						*/
/* - then deletes the next token.						*/
/* - After that, if next->token->type is WORD,			*/
/*   the function calls itself to join the next token	*/
/*	***********************************************		*/
void	join_next_token(t_token *token, char *line, t_minishell *minishell)
{
	char	*new_value;

	new_value = ft_strjoin(token->value, token->next->value);
	if (!new_value)
		error_malloc(line, NULL, minishell, "join_next_token");
	free(token->value);
	token->value = new_value;
	delete_next(token);
	if (token->next && token->next->type == WORD)
		join_next_token(token, line, minishell);
}

/*	****************************************		*/
/* case_pipe:										*/
/*	- sets @cmd_found to false						*/
/*	- deletes the following space if there is one	*/
/*	- checks for syntax errors:						*/
/*			if PIPE is the first token				*/
/*			if PIPE is the last token				*/
/*			or if the next token is a PIPE			*/
/*				-> returns an error					*/
/*	****************************************		*/
t_token	*case_pipe(t_token *token, bool *cmd_found, int *error, t_token **head)
{
	*cmd_found = false;
	if (token->next && token->next->type == ONE_SPACE)
		delete_next(token);
	if (token == *head || !token->next || token->next->type == PIPE)
	{
		ft_printf_fd(2, "minishell: syntax error after %s\n", token->value);
		*error = 3;
		return (token);
	}
	token = token->next;
	return (token);
}

/*	**********************************************		*/
/* parse_tokens and subfunctions:						*/
/*	1.define more precisely the type of the token:		*/
/*		- WORD becomes	IS_CMD, IS_ARG					*/
/*						IS_FILENAME or IS_DELIMITER		*/
/*		- REDIRECTION becomes IS_ INPUT, IS_OUTPUT		*/
/*							  HEREDOC or IS_APPEND		*/
/*		- no change of type for PIPE and ONE_SPACE		*/
/*	2.remove unnecessary spaces:						*/
/*		Only spaces between 2 IS_ARG are left.			*/
/*	3.Check for syntax errors:							*/
/*		- REDIRECTIONS must be followed by IS_FILENAME	*/
/*		- HEREDOC must be followed by IS_DELIMITER		*/
/*		- PIPE must not be followed by another PIPE		*/
/*					not be the last token				*/
/*					not be the first token				*/
/*	**********************************************		*/
int	parse_tokens(char *line, t_token **token_list, t_minishell *minishell)
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
			token = case_redirection(token, &error, line, minishell);
		if (!error && token && token->type == WORD && !cmd_found)
			token = case_command(token, &cmd_found, line, minishell);
		if (!error && token && token->type == WORD && cmd_found)
			token = case_arg(token, line, minishell);
		if (!error && token && token->type == PIPE)
			token = case_pipe(token, &cmd_found, &error, token_list);
	}
	/*ft_printf_fd(1, "token list after parsing:\n");//pour test*/
	/*print_tokens_types(*token_list);//pour test*/
	if (error)
	{
		free_line_and_token_list(line, token_list);
		minishell->exec.error = 2;
	}
	return (error);
}
