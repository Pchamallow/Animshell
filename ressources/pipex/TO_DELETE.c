/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TO_DELETE.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 13:16:41 by pswirgie          #+#    #+#             */
/*   Updated: 2026/03/20 15:03:19 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	print_tab(t_element *type)
{
	int	i;
	int	nb;

	i = 0;
	nb = type->nb_opt + 2;
	if (nb == 0)
		nb = 2;
	while (nb)
	{
		ft_printf_fd(2, "args_execve %s = %s\n", type->file, type->args_execve[i]);
		i++;
		nb--;
	}
}

void	print_structure(t_parse *parse)
{
	int	i;

	i = 0;
	ft_printf_fd(1, "parse->in.file = %s\n", parse->in.file);
	ft_printf_fd(1, "parse->in.cmd = %s\n", parse->in.cmd);
	while (parse->in.options && parse->in.options[i])
	{
		ft_printf_fd(1, "parse->in.option = %s\n", parse->in.options[i]);
		i++;
	}
	i = 0;
	ft_printf_fd(1, "parse->out.file = %s\n", parse->out.file);
	ft_printf_fd(1, "parse->out.cmd = %s\n", parse->out.cmd);
	while (parse->out.options && parse->out.options[i])
	{
		ft_printf_fd(1, "parse->out.option = %s\n", parse->out.options[i]);
		i++;
	}

		i = 0;
	ft_printf_fd(1, "parse->in.file = %s\n", parse->in.cmd_path);
	ft_printf_fd(1, "parse->out.file = %s\n", parse->out.cmd_path);

}
