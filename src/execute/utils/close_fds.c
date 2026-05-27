/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 11:37:45 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/27 11:37:56 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fds_pipe(t_pipe *pipe)
{
	if (pipe->infile && pipe->infile->fd >= 0)
		close(pipe->infile->fd);
	if (pipe->outfile && pipe->outfile->fd >= 0)
		close(pipe->outfile->fd);
}

void	close_fd(int *fd)
{
	if (*fd && *fd >= 0)
	{
		close(*fd);
		*fd = -1;
	}
}
