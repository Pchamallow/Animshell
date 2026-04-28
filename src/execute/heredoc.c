/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:07:23 by pswirgie          #+#    #+#             */
/*   Updated: 2026/04/28 13:51:12 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_permissions(t_minishell *minishell, t_pipe *pipe, int *fd)
{
	if (*fd < 0)
	{
		pipe->output = ERROR;
		printf("error out\n");
		if (pipe->input != ERROR)
			strerror_file("Heredoc failed.");
		minishell->exec.error = 2;
	}
	if (access("minishell_heredoc", W_OK | R_OK) != 0)
	{
		// printf("error out\n");
		// strerror_file(token->value);
		pipe->output = ERROR;
		minishell->exec.error = 1;
		return (1);
	}
	// close(*fd);
	return (0);
}


void	heredoc_delimiter(t_minishell *minishell, char *original)
{
	t_token *hd;
	int		i;
	int		single;
	int		doubleq;
	int		last_single;
	int		last_doubleq;
	bool	open_double;
	bool	open_single;

	i = 0;
	single = 0;
	doubleq = 0;
	open_double = false;
	open_single = false;
	
	hd = minishell->here_doc;

	/*
	1. estce que le texte est entre quotes ?
		entre double quotes  ou single = juste le texte
		juste des double/single quotes = entrer est le delilimiter
		si c est double quotes avec l interieure des singles quotes 
			= 'delilimiter"
		et single quote avec double dedans
			= "delimiter"
		si double quote c double quote, le c est print
	2. estce qu il reste du texte apres les quotes ?
	*/
	
	// pas de texte apres la fin des quotes
	if (original[i] == '\'')
		single++;
	if (original[i] == '\"')
		doubleq++;

	while (original[i])
	{
		if (original[i] && original[i] == '\'')
		{
			single++;
			last_single = i;
			// if (open_double == true && open_single == false)
			
			if (open_single == true)
				open_single == false;
			else
				open_single == true;
		}
		if (original[i] == '\'')
		{
			doubleq++;
			last_doubleq = i;
			if (open_double == true)
				open_double == false;
			else
				open_double == true;
		}
		i++;
	}
	/*
	si j ai que des double quotes ou singles
	et qu il n y pas texte apres
	-> ddlimiter = word without quotes
	*/
	if ((single >= 2 && doubleq == 0 && original[last_single + 1] == '\0')
		|| (doubleq >= 2 && single == 0 && original[last_doubleq + 1] == '\0'))
		//= le mot sans les quotes
	// else if 
	
}



/*
comportement de here doc 

delimiter =
ecire du texte, 
tant qu il n y a pas une ligne avec seulement
le delimiter, on continue

ce texte est le infile de la commande 

comme our les autres infiles, seul le dernier delimiter est 
pris en compte comme infile

delimiter 
1. declencher le readline avec un > en prompt
2. avoir un while on a pas seulement le delimiter
3. mettre le contenu des lignes dans un fichier ? 


- in delimiter, if we have a quote (double or single)
not expand


*/
// init here doc a faire hors du while d execution ?
int	heredoc(t_minishell *minishell, t_pipe *pipe, t_token *token)
{
	t_token	*hd;
	int		*fd;
	bool	to_expand;

	fd = &hd->fd;
	to_expand = true;
	hd = minishell->here_doc;
	
	*fd = open(hd->path_explicite, O_CREAT | O_RDWR | O_TRUNC, 0644);
	
	if (heredoc_permissions(minishell, pipe, fd))
		return (0);
	
	if (ft_strchr(token->value, '\'') || ft_strchr(token->value, '"'))
		to_expand = false;

	// faire un char *delimiter = delimiter sans "" mais avce '' 
	// que si on a des quotes qui englobes
	// heredoc->value = delimiter cleaned
	heredoc_delimiter(minishell, token->value);

		
	// (void)token;
	close(*fd);
	return (0);
	
}
