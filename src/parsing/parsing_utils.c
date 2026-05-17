/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 22:07:46 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/16 22:25:36 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	**********************************************************		*/
/* case_heredoc:													*/
/*	- sets token type to HEREDOC									*/
/*	- deletes the following space if there is one					*/
/*	- checks if the next token is a WORD:							*/
/*				- if no, returns an error							*/
/*				- else, sets the next token type to IS_DELIMITER	*/
/*	- if IS_DELIMITER->next token is a WORD:						*/
/*				- joins next token									*/
/*	- deletes the space following IS_DELIMITER if there is one		*/
/*	- returns the token after IS_DELIMITER							*/
/*	**********************************************************		*/
t_token	*case_heredoc(t_token *token, int *error, char *line,
		t_minishell *minishell)
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
	if (token->next && token->next->type == WORD)
		join_next_token(token, line, minishell);
	if (token->next && token->next->type == ONE_SPACE)
		delete_next(token);
	token = token->next;
	return (token);
}

static void	set_redirection_type(t_token *token)
{
	if (ft_strncmp(token->value, ">>", 2) == 0)
		token->type = IS_APPEND;
	else if (ft_strncmp(token->value, "<", 1) == 0)
		token->type = IS_INPUT;
	else if (ft_strncmp(token->value, ">", 1) == 0)
		token->type = IS_OUTPUT;
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
/*	- if IS_FILENAME->next token is a WORD:			*/
/*				- joins next token					*/
/*	- removes the space following IS_FILENAME		*/
/*	- returns the token following IS_FILENAME		*/
/*	*******************************************		*/
t_token	*case_redirection(t_token *token, int *error, char *line,
		t_minishell *minishell)
{
	if (ft_strncmp(token->value, "<<", 2) == 0)
	{
		token = case_heredoc(token, error, line, minishell);
		return (token);
	}
	set_redirection_type(token);
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
	if (token->next && token->next->type == WORD)
		join_next_token(token, line, minishell);
	if (token->next && token->next->type == ONE_SPACE)
		delete_next(token);
	token = token->next;
	return (token);
}

/*	*********************************		*/
/* case_command:							*/
/*	- sets the token type to IS_CMD			*/
/*	- sets @cmd_found to true				*/
/*	- if next token is a WORD:				*/
/*				- joins next token			*/
/*	- removes the following space if needed	*/
/*	- returns the next token				*/
/*	*********************************		*/
t_token	*case_command(t_token *token, bool *cmd_found, char *line,
		t_minishell *minishell)
{
	*cmd_found = true;
	if (token->next && token->next->type == WORD)
		join_next_token(token, line, minishell);
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
/*	- if IS_ARG->next token is a WORD:								*/
/*				- joins next token									*/
/*	- checks if the next token is ONE_SPACE							*/
/*			and the next next token WORD							*/
/* 					-> if yes, keeps the space						*/
/* 							and returns the next next token			*/
/* 					-> if no, removes the space if there is one		*/
/* 							and returns the next token				*/
/*	*************************************************************	*/
t_token	*case_arg(t_token *token, char *line, t_minishell *minishell)
{
	token->type = IS_ARG;
	if (token->next && token->next->type == WORD)
		join_next_token(token, line, minishell);
	if (token->next && token->next->type == ONE_SPACE)
		delete_next(token);
	token = token->next;
	return (token);
}
