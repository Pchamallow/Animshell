/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_gnl.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 12:58:13 by pswirgie          #+#    #+#             */
/*   Updated: 2026/02/07 12:58:13 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	end_gnl(char *line, int fd, int end)
{
	free(line);
	line = get_next_line(fd);
	if (!line)
		return (close(fd), 0);
	while (end--)
	{
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}

int	end_file(char *file)
{
	int		fd;
	char	*line;
	int		end;

	end = 0;
	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (1);
	line = get_next_line(fd);
	if (!line)
		return (close(fd), 0);
	while (line)
	{
		end++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (end);
}
