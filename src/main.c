/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:07:23 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/04 17:56:55 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	main(void)
{
	t_minishell	minishell;
	
	minishell.token.value ="coucou";
	if (execute(&minishell) == 1)
		return (1);

	return (0);

}
