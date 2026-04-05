/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 21:18:24 by stkloutz          #+#    #+#             */
/*   Updated: 2026/04/05 23:08:55 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*case_redirection(t_token *token, bool *cmd_found)
{
	return (token);
}

t_token *case_heredoc(t_token *token, bool *cmd_found)
{
	return (token);
}

t_token *case_command(t_token *token, bool *cmd_found)
{
	*cmd_found = true;
	if (token->value == "echo" || token->value == "cd" || token->value== "pwd"
			|| token->value == "export" || token->value == "unset"
			|| token->value == "env" || token->value == "exit")
		token->type = IS_BUILT_IN;
	else
		token->type = IS_CMD;
	token = token->next;

	return (token);
}

int	parsing(t_token **token_list)
{
	t_token	token;
	bool	cmd_found;//pareil avec file in, file out, heredoc -> struct de bool ?

	token = *token_list;
	while (token)
	{
		cmd_found = false;
		//first token
		if (token->type == ONE_SPACE)
			token = token->next;
		if (!token)
			break;
		if (token->type == PIPE)
		{
			ft_printf_fd(2, "syntax error\n");
			return (1);
		}
		if (token->type == REDIRECTION)
			token = case_redirection(&token, &cmd_found);
		if (token->type == WORD)
			token = case_command(&token, &cmd_found);
		if (token->type == HEREDOC)
			token = case_heredoc(&token, &cmd_found);
		//si pipe : check next token != NULL
	}

}
