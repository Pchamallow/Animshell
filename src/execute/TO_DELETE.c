
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

void	print_pauline(t_minishell *minishell)
{
	t_token *token = minishell->token;
    
	ft_printf_fd(2, "\n");
	while (token != NULL)
	{
		ft_printf_fd(2, "%s\n", token->value);
		token= token->next;
	}
	ft_printf_fd(2, "\nbuilt in : %d\n", minishell->exec.built_in);
	ft_printf_fd(2, "\ncmd : %s\n", minishell->exec.pipe_a->value);
	ft_printf_fd(2, "\ncmd args :\n");
	print_double(minishell->exec.pipe_a->cmd_args);
	
}
