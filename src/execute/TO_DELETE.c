
#include "minishell.h"

void print_double(char **str)
{
	int i;

	i = 0;
	while (str[i])
	{
		ft_printf_fd(2, "%s\n", str[i]);
		i++;
	}
}

void print_pauline_pipe(t_pipe *pipe)
{
	ft_printf_fd(2, "\nbuilt in : %d\n", pipe->builtin_kind);
	if (pipe->cmd)
	{
		ft_printf_fd(2, "\ncmd : %s\n", pipe->cmd->value);
		ft_printf_fd(2, "\ncmd args :\n");
		if (pipe->cmd->cmd_args)
			print_double(pipe->cmd->cmd_args);
		else 
			ft_printf_fd(2, "NONE\n");
	}
	if (pipe->infile && pipe->infile->value)
		ft_printf_fd(2, "infile : %s\n", pipe->infile->value);
	if (pipe->outfile && pipe->outfile->value)
		ft_printf_fd(2, "outfile : %s\n", pipe->outfile->value);
	ft_printf_fd(2, "------------------------\n");
}

void	print_pauline(t_minishell *minishell)
{
	t_token *token = minishell->token;
	
	ft_printf_fd(2, "\n--------------print----------------------\n");
	while (token != NULL)
	{
		ft_printf_fd(2, "%s\n", token->value);
		token= token->next;
	}
	ft_printf_fd(2, "\n\npipe_lst\n");
	while (minishell->exec.pipe_lst)
	{
		print_pauline_pipe(minishell->exec.pipe_lst);
		minishell->exec.pipe_lst = minishell->exec.pipe_lst->next;
	}
}

void	print_pipefd(int fd1, int fd2)
{
	(void)fd1;
	(void)fd2;
	printf("close pipefd[0]\n");
	printf("close pipefd[1]\n");
	// if (fd1)
	// 	printf("pipefd[0] read = %d\n", fd1);
	// if (fd2)
	// 	printf("pipefd[1] write = %d\n", fd2);
}

void	print_tokens_types(t_token *token)// pour tester
{
	char	*str[] = {"word", "is_cmd", "is_built_in", "is_arg", "is_filename",
		"is_delimiter", "space", "pipe", "redirection", "is input",
		"is_output", "is_append", "heredoc"};
	/*char *quote[] = {"no", "single", "double"};*/
	while (token)
	{
		/*ft_printf_fd(1, "%s	type=%s quote=%s\n", token->value,*/
				/*str[token->type], quote[token->quote]);*/
		ft_printf_fd(1, "%s		type=%s\n", token->value,
			str[token->type]);
		token = token->next;
	}
}

//read_token:
bool is_redirection(t_token *token)
{
	if (ft_strchr(token->value, '<') == NULL || ft_strchr(token->value, '>') == NULL)
		return (true);
	return (false);
}

void cpy_no_bzero(char *dst, const char *src, size_t size)
{
	size_t i;

	i = 0;
	while (i < (size - 1) && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
}

void convert_to_single_quotes(t_minishell *minishell, t_token *token)
{
	t_token *tmp_modify;
	char *original;
	int len;

	len = ft_strlen(token->value);
	tmp_modify = token;
	original = ft_calloc(len + 1, sizeof(char));
	if (!tmp_modify->value)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	ft_strlcpy(original, token->value, len);
	free(tmp_modify->value);
	tmp_modify->value = ft_calloc(len + 1, sizeof(char));
	if (!tmp_modify->value)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	tmp_modify->value[0] = '\'';
	cpy_no_bzero(tmp_modify->value, &original[1], len - 1);
	tmp_modify->value[len] = '\'';
	free(original);
}

void	remove_quots(t_minishell *minishell, t_token *token)
{
	char	*original;
	int		len;

	original = ft_strdup(token->value);
	if (!original)
		print_error_free(minishell, "Malloc failed.\n", EXIT_FAILURE);
	len = ft_strlen(original);
	free(token->value);
	token->value = ft_calloc(len, sizeof(char));
	ft_strlcpy(token->value, &original[1], len - 1);
	free(original);
	// printf("resultat = %s\n", token->value);
}

int	is_double_quoted(char *str)//peut-etre plus utile puisque le parsing ne garde plus les quotes
{
	int	i;
	int	doubled;

	i = 0;
	doubled = 0;
	while (str[i])
	{
		if (str[i] == '"')
			doubled++;
		i++;
	}
	if (str[i] == '\0' && doubled == 2)
		return (1);
	return (0);
}

bool is_single_double_quoted(t_minishell *minishell, t_token *token)
{
	char	*str;
	int		i;
	int		single;
	int		doubled;

	i = 0;
	single = 0;
	doubled = 0;
	str = token->value;
	if (token->quote == SINGLE && is_double_quoted(token->value))
		return (false);
	if (str[0] == '\0')
		return (false);
	if (str[i] == '\'')
		single++;
	if (str[i] == '"')
		doubled++;
	i++;
	while (str[i])
	{
		if (str[i] == '\'')
			single = i;
		if (str[i] == '"')
			doubled = i;
		i++;
	}
	if (single > 1 && str[i] == '\'')
		remove_quots(minishell, token);
	if (doubled > 1 && str[i] == '\"')
		remove_quots(minishell, token);
	return (false);
}

