
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