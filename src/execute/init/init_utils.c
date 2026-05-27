/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkloutz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 17:41:04 by stkloutz          #+#    #+#             */
/*   Updated: 2026/05/27 16:44:59 by stkloutz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cpy_tab_cmd(char *str, t_pipe *pipe)
{
	int	len;

	len = ft_strlen(pipe->cmd->value) + 2;
	ft_strlcpy(str, pipe->cmd->value, len);
	return ;
}

int	is_cmd(t_token *words)
{
	t_token	*token;

	token = words;
	while (token)
	{
		if (token->type == IS_CMD || token->type == IS_BUILT_IN)
			return (1);
		else if (token->type == PIPE)
			break ;
		token = token->next;
	}
	return (0);
}

bool	is_pipe(t_pipe *pipe, t_token *token)
{
	if (token->type == PIPE)
	{
		if (pipe->output == TERMINAL)
			pipe->output = IS_PIPE;
		return (true);
	}
	return (false);
}
