
#include "minishell.h"

void print_double(char **str)
{
	int i;

	i = 0;
	while (str[i] != NULL)
	{
		ft_printf_fd(2, "%s\n", str[i]);
		i++;
	}
}

void print_pauline_type(t_pipe *pipe)
{
	if (pipe->infile && pipe->infile->value)
		ft_printf_fd(2, "%s -> infile : %s\n", pipe->cmd->value, pipe->infile->value);
	if (pipe->outfile && pipe->outfile->value)
		ft_printf_fd(2, "%s -> outfile : %s\n", pipe->cmd->value, pipe->outfile->value);
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
	ft_printf_fd(2, "\nbuilt in : %d\n", minishell->exec.pipe_a->built_in);
	ft_printf_fd(2, "\ncmd : %s\n", minishell->exec.pipe_a->cmd->value);
	ft_printf_fd(2, "\nTABLEAU :\n");
	print_double(minishell->exec.pipe_a->cmd->cmd_args);
	ft_printf_fd(2, "------------------\n");
	print_pauline_type(minishell->exec.pipe_a);
	print_pauline_type(minishell->exec.pipe_b);
}
