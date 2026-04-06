/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <stkloutz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 21:18:24 by stkloutz          #+#    #+#             */
/*   Updated: 2026/04/06 21:52:56 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_space(t_token *token)
{
	t_token *tmp;

	tmp = token->next;
	token->next = token->next->next;
	ft_token_delone(tmp, free);
	return;
}

t_token	*case_redirection(t_token *token, int *error)
{
	if (ft_strncmp(token->value, "<", 1) == 0)
		token->type = IS_INPUT;
	if (ft_strncmp(token->value, ">", 1) == 0)
		token->type = IS_OUTPUT;
	if (ft_strncmp(token->value, ">>", 2) == 0)
		token->type = IS_APPEND;
	if (token->next && token->next->type == ONE_SPACE)
		delete_space(token);
	if (!token->next || token->next->type != WORD)
	{
		ft_printf_fd(2, "minishell: syntax error after %s\n", token->value);
		*error = 1;
		return (token);
	}
	token = token->next;
	token->type = IS_FILENAME;
	if (token->next && token->next->type == ONE_SPACE)
		delete_space(token);
	token = token->next;
	return (token);
}

t_token *case_heredoc(t_token *token, int *error)
{
	if (token->next && token->next->type == ONE_SPACE)
		delete_space(token);
	if (!token->next || token->next->type != WORD)
	{
		*error = 2;
		ft_printf_fd(2, "minishell: syntax error after %s\n", token->value);
		return (token);
	}
	token = token->next;
	token->type = IS_DELIMITER;
	if (token->next && token->next->type == ONE_SPACE)
		delete_space(token);
	token = token->next;
	return (token);
}

t_token *case_command(t_token *token, bool *cmd_found)
{
	*cmd_found = true;
	if (ft_strnstr(CMD_LIST, token->value, ft_strlen(CMD_LIST)) != NULL)
		token->type = IS_BUILT_IN;
	else
		token->type = IS_CMD;
	if (token->next && token->next->type == ONE_SPACE)
		delete_space(token);
	token = token->next;
	return (token);
}

t_token *case_arg(t_token *token)
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
		delete_space(token);
	token = token->next;
	return (token);
}

t_token *case_pipe(t_token *token, bool *cmd_found, int *error)
{
	*cmd_found = false;
	if (token->next && token->next->type == ONE_SPACE)
		delete_space(token);
	if (!token->next || token->next->type == PIPE)
	{
		ft_printf_fd(2, "minishell: syntax error after %s\n", token->value);
		*error = 3;
		return (token);
	}
	token = token->next;
	return (token);
}

int	parse_tokens(t_token **token_list)
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
		if (!error && token && token->type == HEREDOC)
			token = case_heredoc(token, &error);
		if (!error && token && token->type == WORD && !cmd_found)
			token = case_command(token, &cmd_found);
		if (!error && token && token->type == WORD && cmd_found)
			token = case_arg(token);
		if (!error && token && token->type == PIPE)
			token = case_pipe(token, &cmd_found, &error);
	}
	ft_printf_fd(1, "token list after parsing:\n");//pour test
	print_tokens_types(*token_list);//pour test
	return (error);
}
