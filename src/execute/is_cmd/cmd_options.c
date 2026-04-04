/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_options.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 18:53:11 by pswirgie          #+#    #+#             */
/*   Updated: 2026/03/28 13:35:01 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	cpy_cmd(t_parse *parse, t_element *type, char **cmd, char *str)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	j = 0;
	len = len_cmd(str);
	(void)type;
	if (len == 1)
		len = 2;
	else if (type->nb_opt > 0)
		len = len_cmd_no_endspace(str);
	*cmd = (char *)malloc(sizeof(char) * len + 1);
	if (!*cmd)
		print_error_free(parse, "Malloc failed.\n", EXIT_FAILURE);
	ft_bzero(*cmd, len + 1);
	while (str[i] && len)
	{
		(*cmd)[j] = str[i];
		j++;
		i++;
		len--;
	}
}

int	len_option(char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		i++;
		if (src[i] == ' ' || (is_sign(src[i]) == 1))
			return (i);
	}
	if (i == 0)
		i++;
	return (i);
}

void	cpy_options(char **options, char *src)
{
	int	i;
	int	j;
	int	len;
	int	option;

	j = 0;
	i = len_cmd_no_endspace(src) + 1;
	option = is_option(src);
	while (option)
	{
		len = len_option(&src[i]) + 1;
		ft_strlcpy(options[j], &src[i], len);
		i = i + len;
		j++;
		option--;
	}
}

void	init_cmd_and_options(int options, char *arg,
	t_element *type, t_parse *parse)
{
	int	len;

	len = len_cmd(arg);
	if (len == 0)
		type->is_cmd = 0;
	else if (type->is_file == 1)
	{
		if (options > 0)
		{
			cpy_cmd(parse, type, &type->cmd, arg);
			init_options(arg, type, parse);
		}
		else
			cpy_cmd(parse, type, &type->cmd, arg);
	}
}

int	is_option(char *str)
{
	int	i;
	int	cmd;
	int	option;

	option = 0;
	cmd = 0;
	i = is_space(str, 0);
	if (str[i] == '.' && str[i + 1] == '/')
		i += 2;
	while (str[i] && str[i] != ' ')
	{
		cmd = 1;
		i++;
	}
	i += is_space(&str[i], 0);
	while (str[i] && cmd == 1)
	{
		if (str[i] != ' ')
		{
			option++;
			i += is_space(&str[i], 1);
		}
		i += is_space(&str[i], 0);
	}
	return (option);
}
