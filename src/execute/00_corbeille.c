static int    get_username(t_env *env)
{
    const char *user;
	const char *name;
	int len_user;
	int len_name;

	user = getenv("USER");
	name = getenv("NAME");
	// pour 42, anme est remplacer par session manager apres local/
	ft_printf_fd(2, "user : %s\n", name);
	if (!user || !name)
		return (1);
	len_user = ft_strlen(user);
	len_name = ft_strlen(name);
	env->username = ft_calloc((len_user + len_name + 4), sizeof(char));
	ft_strlcpy(env->username, user, len_user + 1);
	env->username[len_user] = '@';
	ft_strlcpy(&env->username[len_user + 1], name, len_name + 1);
	env->username[len_user + len_name + 1] = '$';
	env->username[len_user + len_name + 2] = ' ';
	// str[0][0] = '1';
	ft_printf_fd(2, "ici = %s\n", env->username);
	
	return (0);
}

void	free_struct_env(t_env *env)
{
	if (env->username)
		free(env->username);
}