
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
	ft_printf_fd(2, "\nbuilt in : %d\n", pipe->built_in);
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
