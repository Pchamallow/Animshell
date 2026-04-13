/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 22:07:46 by stkloutz          #+#    #+#             */
/*   Updated: 2026/04/13 16:19:53 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	****************************************************		*/
/* case_heredoc:												*/
/*	- sets token type to HEREDOC								*/
/*	- deletes the following space if there is one				*/
/*	- checks if the next token is a WORD:						*/
/*				- if no, returns an error						*/
/*				- else, sets the next token type to IS_DELIMITER*/
/*	- deletes the space following IS_DELIMITER if there is one	*/
/*	- returns the token after IS_DELIMITER						*/
/*	****************************************************		*/
t_token	*case_heredoc(t_token *token, int *error)
{
	token->type = HEREDOC;
	if (token->next && token->next->type == ONE_SPACE)
		delete_next(token);
	if (!token->next || token->next->type != WORD)
	{
		*error = 2;
		ft_printf_fd(2, "minishell: syntax error after %s\n", token->value);
		return (token);
	}
	token = token->next;
	token->type = IS_DELIMITER;
	if (token->next && token->next->type == ONE_SPACE)
		delete_next(token);
	token = token->next;
	return (token);
}

/*	*******************************************		*/
/* case_redirection: 								*/
/*	- precises the redirection type:				*/
/*		- << IS_HEREDOC (specific function)			*/
/*		- >> IS_APPEND								*/
/*		- > IS_OUTPUT								*/
/*		- < IS_INPUT								*/
/*	- removes the following space if there is one	*/
/*	- checks if the next token is a WORD			*/
/*			(if no, returns an error)				*/
/*		and sets its type to IS_FILENAME			*/
/*	- removes the space following IS_FILENAME		*/
/*	- returns the token following IS_FILENAME		*/
/*	*******************************************		*/
t_token	*case_redirection(t_token *token, int *error)
{
	if (ft_strncmp(token->value, "<<", 2) == 0)
	{
		token = case_heredoc(token, error);
		return (token);
	}
	else if (ft_strncmp(token->value, ">>", 2) == 0)
		token->type = IS_APPEND;
	else if (ft_strncmp(token->value, "<", 1) == 0)
		token->type = IS_INPUT;
	else if (ft_strncmp(token->value, ">", 1) == 0)
		token->type = IS_OUTPUT;
	if (token->next && token->next->type == ONE_SPACE)
		delete_next(token);
	if (!token->next || token->next->type != WORD)
	{
		ft_printf_fd(2, "minishell: syntax error after %s\n", token->value);
		*error = 1;
		return (token);
	}
	token = token->next;
	token->type = IS_FILENAME;
	if (token->next && token->next->type == ONE_SPACE)
		delete_next(token);
	token = token->next;
	return (token);
}

/*	*********************************		*/
/* case_command:							*/
/*	- sets the token type to IS_CMD			*/
/*	- sets @cmd_found to true				*/
/*	- removes the following space if needed	*/
/*	- returns the next token				*/
/*	*********************************		*/
t_token	*case_command(t_token *token, bool *cmd_found)
{
	*cmd_found = true;
	if (find_built_in(token->value) == true)
		token->type = IS_BUILT_IN;
	else
		token->type = IS_CMD;
	if (token->next && token->next->type == ONE_SPACE)
		delete_next(token);
	token = token->next;
	return (token);
}

/*	*************************************************************	*/
/* case_arg:														*/
/*	- sets the token type to IS_ARG									*/
/*	- checks if the next token is ONE_SPACE							*/
/*			and the next next token WORD							*/
/* 					-> if yes, keeps the space						*/
/* 							and returns the next next token			*/
/* 					-> if no, removes the space if there is one		*/
/* 							and returns the next token				*/
/*	*************************************************************	*/
t_token	*case_arg(t_token *token)
{
	t_token	*check_next;

	token->type = IS_ARG;
	check_next = token->next;
	if (check_next && check_next->type == ONE_SPACE
		&& check_next->next && check_next->next->type == WORD)
	{
		token = check_next->next;
		return (token);
	}
	if (check_next && check_next->type == ONE_SPACE)
		delete_next(token);
	token = token->next;
	return (token);
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
