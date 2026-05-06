/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pswirgie <pswirgie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 17:10:58 by pswirgie          #+#    #+#             */
/*   Updated: 2026/05/06 18:16:54 by pswirgie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
copy a new
free new
malloc new with a new len
*/
// CONVERTIR LE NEW EN ** POUR NE PAS EN FAIRE UNE COPIE SEULEEMNT
// char	*str_cpyinit(char *original, char *new, int new_len)
// {
// 	if (new)
// 	{
// 		original = ft_strdup(new);
// 		if (!original)
// 			return (NULL);
// 		free(new);
// 		new = ft_calloc(new_len, sizeof(char));
// 		if (!new)
// 		{
// 			free(original);
// 			return (NULL);
// 		}
// 		ft_printf_fd(2, " c est bien copi'e = %s\n", original);
// 		return (new);
// 	}
// 	return (NULL);
// }

static void	path_kind(t_minishell *minishell, char **path)
{
	char	*new_path;

	if ((*path)[0] == '.' && (*path)[1] == '.' && (*path)[2] == '/')
		minishell->builtin.cd.path = RELATIVE_DOUBLE;
	else if ((*path)[0] == '.' && (*path)[1] == '/')
		minishell->builtin.cd.path = RELATIVE_SINGLE;
	else if ((*path)[0] == '/')
		minishell->builtin.cd.path = ABSOLUTE;
	else if (ft_isalpha((*path)[0]))
	{
		new_path = ft_strjoin("./", *path);
		if (!new_path)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		free(*path);
		*path = new_path;
		minishell->builtin.cd.path = RELATIVE_SINGLE;
	}
}

static char	**init_new_envp(char **envp)
{
	int		len;
	char	**new_envp;

	len = 0;
	while (envp[len])
		len++;
	new_envp = ft_calloc(len + 1, sizeof(char *));
	if (!new_envp)
		return (NULL);
	new_envp[len] = NULL;
	return (new_envp);
}

char	*path_replacefolder(char *old_pwd, char **path)
{
	char	*new_pwd;
	// char	*new_path;
	char	*tmp;
	int		max_old;

	max_old = ft_strlen(old_pwd) - 1; // verifier qu il va jusqu bout
	// if (!find_ascii(old_pwd))
		//suppr le dernier folder sans le remplacer
		// return
	new_pwd = NULL;
	// le / de fin n est pas cens'e exister, il ne doit pas il y avoir
	while (old_pwd[max_old]
		&& old_pwd[max_old] != '/')
		max_old--;
	if (max_old > 0)
	{
		new_pwd = ft_substr(old_pwd, 0, max_old);
		// new_path = ft_substr
		tmp = ft_strdup(new_pwd);
		free(new_pwd);
		new_pwd = ft_strjoin(tmp, *path);
	}
	return (new_pwd);
}

// char	*replace_str_by_chr(char *path, char *to_replace, char c)
// {
// 	char	*tmp;
// 	char	*new;
// 	// int	i;
// 	int		here;

// 	//remove // s il y en a un a la fin
// 	(void)c;
// 	tmp = ft_strnstr(path, to_replace, ft_strlen(path));
// 	here = tmp - path - 2;
// 	new = NULL;
// 	if (here == 0)
// 		new = ft_strdup(path);
// 		//ne pas suppr le double //
// 	printf("here = %d\n", here);
// 		// dans path[i] chercher la string to_replace
// 		// avoir un index au moins
// 		// lorsqu elle est trouver, remplacer par c 
// 		// rappeler la fonction jusqu' a que toutes la string soit clean
// 	return (tmp);
// }


int	remove_begin(char **str, char c)
{
	char	*new;
	int		end;
	int		i;

	i = 0;
	end = 0;
	if ((*str)[0] == '.' && (*str)[1] == '/')
	{
		i += 2;
		end = i;
		while ((*str)[i] && (*str)[i] == c)
		{
			i++;
			end++;
		}
	}
	end--;
	if (end == 0)
		return (0);
	new = ft_substr(*str, end, ft_strlen(*str) - end);
	if (!new)
		return (1);
	printf("new == %s\n", new);
	free(*str);
	*str = new;
	return (0);
}

int	remove_end(char **str, char c)
{
	char	*new;
	int		end;

	end = ft_strlen(*str) - 1;
	while (end > 0 && (*str)[end] && (*str)[end] == c)
		end--;
	end++;
	new = ft_substr(*str, 0, end);
	if (!new)
		return (1);
	free(*str);
	*str = new;
	return (0);
}

int	has_alpha(char *str)
{
	int	alpha;
	int	end;
	int	i;

	i = 0;
	alpha = 0;
	end = ft_strlen(str) - 1;
	if (end <= 0)
		return (0);
	while (str[end] && end > 0)
	{
		if (ft_isalpha(str[end]))
		{
			alpha = 1;
			break;
		}
		end--;
	}
	return (alpha);
}

/*
** PATH_CLEAN *********************************************
** 1. if needed, remove several / 
** 		- if we have a alpha remove the / or // at end of path,
		- begin .// -> /
** 2. clean the path by kend
**		- absolute (path = /home/documents)
**			new_pwd = "PWD=" + path
**		- relative single (path = ./documents)
**			new_pwd = old_pwd + path (without ./)
**		- relative double (path = ../documents)
**			new_pwd = old_pwd (without last folder)
**						+ path (without ./)
** 
*/
static int	path_clean(t_minishell *minishell, char **path, int index_pwd)
{
	t_path_kind kind;
	char		*new_path;
	char		*old_pwd;

	new_path = NULL;
	kind = minishell->builtin.cd.path;
	if (has_alpha(*path))
	{
		if (remove_begin(path, '/') || remove_end(path, '/'))
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	}
	printf("kind == %d\n", kind);
	if (kind == ABSOLUTE)
	{
		new_path = ft_strjoin("PWD=", *path);
		if (!new_path)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		free(*path);
		*path = new_path;
	}
	else if (kind == RELATIVE_SINGLE)
	{
		old_pwd = ft_strdup(minishell->exec.envp[index_pwd]);
		if (!old_pwd)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		new_path = ft_strjoin(old_pwd, *path);
		if (!new_path)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		free(old_pwd);
		free(*path);
		*path = new_path;
	}
	// else if (kind == RELATIVE_DOUBLE)
	// {
	// 	// faire aussi si je n ai rien apres /
	// 	// new_path = path_replacefolder();
	// }
	return (0);
}

static void	replace_pwd(t_minishell *minishell, char **path)
{
	char	**new_envp;
	int		index_pwd;
	int		i;

	i = 0;
	index_pwd = find_pwd(minishell);
	path_clean(minishell, path, index_pwd);
	if (index_pwd != -1)
	{
		new_envp = init_new_envp(minishell->exec.envp);
		while (i < index_pwd)
		{
			new_envp[i] = minishell->exec.envp[i];
			i++;
		}
		free(minishell->exec.envp[index_pwd]);
		new_envp[index_pwd] = *path;
		// printf("new envp good = %s\n", new_envp[index_pwd]);
		i++;
		while (minishell->exec.envp[i])
		{
			new_envp[i] = minishell->exec.envp[i];
			i++;
		}
		free(minishell->exec.envp);
		minishell->exec.envp = new_envp;
		// printf("new envp effective = %s\n", minishell->exec.envp[index_pwd]);
		
	}
}


/*
** CD *********************************************
** Move the current position in a directory
** specified.
**
** - only a relative or absolute path, like required
** by the subject
**
** - no argument or > 1= error message
**
** - if argument is valid
** 		- we move in folder
**		- replace pwd
**			- path is absolute = /newpath
**				-> copy it
**			- path is relative 
**				= ./newpath
**				-> concatenate with the old path
**				= ../newpath
**				-> replace last folder of old path
**					by new path
** 
*/
int	cd(t_minishell *minishell, t_pipe *pipe)
{
	char	*path;
	int		error;

	path = NULL;
	if (!pipe->cmd->cmd_args || !pipe->cmd->cmd_args[0])
	{
		error_cmd_args("cd", NULL, "require one argument");
		minishell->exec.error = 1;
	}
	else if (pipe->cmd->cmd_args[1])
	{
		error_cmd_args("cd", NULL, "too many arguments");
		minishell->exec.error = 1;
	}
	else if (pipe->cmd->cmd_args[0])
	{
		path = ft_strdup(pipe->cmd->cmd_args[0]);
		if (!path)
			print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
		ft_printf_fd(2, "strdup path = %s\n", path);
		// if (ft_strcmp(path, "//") == 0)
		if (ft_strcmp(path, "./") == 0)
			return (0);
		path_kind(minishell, &path);
		ft_printf_fd(2, " c est bien restitue = %s\n", path);
		error = chdir(path);
		if (error != 0)
		{
			ft_printf_fd(2, "minishell: cd: ");
			perror(path);
			minishell->exec.error = 1;
		}
		else
			replace_pwd(minishell, &path);
		ft_printf_fd(2, "error = %d\n", error);
		ft_printf_fd(2, "path = %s\n", path);
		// free(path);
	}
	return (0);
}
