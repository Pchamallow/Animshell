/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 16:07:23 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/04 10:19:11 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

///to delete
#include <stdio.h>

int	main(void)
{
	const char *name = "PATH";

    const char *env_p = getenv(name);

	if(env_p)
		printf("Your %s is %s\n", name, env_p);
	return 0;
}
