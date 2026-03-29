/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 10:21:31 by pswirgie          #+#    #+#             */
/*   Updated: 2026/03/12 15:01:27 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*find_char(char *s, char c)
{
	int		i;
	int		j;
	char	*a;
	char	to_find;

	i = 0;
	j = ft_strlen(s);
	a = (char *)s;
	to_find = (char) c;
	while (i < j)
	{
		if (a[i] == to_find)
			return (&a[i]);
		i++;
	}
	return (NULL);
}

char	*ft_content_line(int fd, char *storage, char *buff)
{
	size_t	bytes_read;
	char	*temp;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buff, BUFFER_SIZE);
		if ((int)bytes_read == -1)
		{
			ft_bzero(storage, ft_strlen(storage));
			return (NULL);
		}
		if (bytes_read == 0)
			break ;
		if (!storage)
			storage = ft_strdup("");
		buff[bytes_read] = '\0';
		temp = storage;
		storage = ft_strjoin(temp, buff);
		free(temp);
		if (find_char(buff, '\n'))
			break ;
	}
	return (storage);
}

char	*ft_leftover(char *line)
{
	size_t	i;
	char	*storage;

	i = 0;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	if (line[i] == '\0')
		return (NULL);
	storage = ft_substr(line, i + 1,
			(ft_strlen((const char *)line) - i));
	if (!*storage)
	{
		free(storage);
		return (NULL);
	}
	if (!storage)
		return (NULL);
	line[i + 1] = '\0';
	return (storage);
}

char	*get_next_line(int fd)
{
	static char	*storage[1024];
	char		*buff;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buff = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buff)
		return (NULL);
	line = ft_content_line(fd, storage[fd], buff);
	free(buff);
	if (!line)
		return (NULL);
	storage[fd] = ft_leftover(line);
	return (line);
}
