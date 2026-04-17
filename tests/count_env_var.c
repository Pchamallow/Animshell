#include <string.h>
#include <stdbool.h>
#include <stdio.h>

int exit_error = 0;

bool	is_whitespace(char c)
{
	if (c == ' ' || c == '\t')
		return (true);
	return (false);
}

int	count_env_var(char *line, int len, int i, char **envp)
{
	int	j;
	int	wd_len;
	int	count;

	count = 0;
	while (i < len)
	{
		if (line[i] == '$' && i + 1 < len)
		{
			if (line[i + 1] == '?')
			{
				//$?
				if (exit_error < 10)
					count += 1;
				else if (exit_error >= 100)
					count += 3;
				//else
				//	exit error: format d'erreur inattendu
				i += 2;
			}
			else if (!is_whitespace(line[i + 1]))
			{
				wd_len = 0;
				//count $ENV:
				//1.find word:
				i++;
				while (i + wd_len < len && !is_whitespace(line[i + wd_len])
							&& line[i + wd_len] != '$')
					wd_len++;
				//2.find word in envp:
				j = 0;
				while (envp[j] && strncmp(line + i, envp[j], wd_len) != 0)
					j++;
				//3.count char:
				if (envp[j])
				{
					count += count_env_var(envp[j] + wd_len + 1,
						strlen(envp[j] + wd_len + 1), 0, envp);
				}
			}
		}
		i++;
	}
	return (count + i);
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	int count;

	count = count_env_var(av[1], strlen(av[1]), 0, envp);
	printf("%d\n", count);

	return (0);
}
