/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 13:26:56 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/20 11:46:02 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_getcwd_error(char **newpwd, t_env **env)
{
	char	*tmp;

	ft_putstr_fd("cd: error retrieving current directory: ", 2);
	perror("getcwd");
	tmp = get_my_env("PWD", *env);
	*newpwd = ft_strjoin(tmp, "/..");
}

int	ft_cd(char **args, t_env **env)
{
	char	*oldpwd;
	char	*newpwd;
	char	*path;

	path = args[1];
	oldpwd = getcwd(NULL, 0);
	if (!path || ft_strcmp(path, "~") == 0)
		path = get_my_env("HOME", *env);
	if (!path)
		return (free(oldpwd), ft_putstr_fd("cd: HOME not set\n", 2), 1);
	if (chdir(path) == -1)
		return (ft_putstr_fd("cd: ", 2), write(2, path, ft_strlen(path)), \
		ft_putstr_fd(" No such file or directory\n", 2), free(oldpwd), 1);
	newpwd = getcwd(NULL, 0);
	if (!oldpwd && !newpwd)
		handle_getcwd_error(&newpwd, env);
	updat_env(env, "OLDPWD", oldpwd);
	updat_env(env, "PWD", newpwd);
	if (oldpwd)
		free (oldpwd);
	return (free(newpwd), 0);
}

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int	ft_exit(char **args)
{
	ft_putstr_fd("exit\n", 1);
	if (!args || !args[1])
		exit (0);
	if ((is_numeric(args[1])) == 1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(":`exit: numeric argument required\n", 2);
		exit (2);
	}
	else if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	exit (ft_atoi(args[1]) % 256);
}

int	ft_pwd(t_env **env)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
	{
		path = get_my_env("PWD", *env);
		if (!path)
			return (1);
	}
	write(1, path, ft_strlen(path));
	write(1, "\n", 1);
	if (path && path != get_my_env("PWD", *env))
		free(path);
	return (0);
}
