/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 13:26:56 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/29 09:42:56 by aelbouz          ###   ########.fr       */
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

char	*resolve_cd_path(char **args, t_env **env, char **upgraded)
{
	char	*path;

	*upgraded = NULL;
	path = args[1];
	if (!path || ft_strcmp(path, "~") == 0)
		path = get_my_env("HOME", *env);
	else if (path[0] == '~' && path[1] == '/')
	{
		*upgraded = ft_strjoin(get_my_env("HOME", *env), path + 1);
		path = *upgraded;
	}
	return (path);
}

int	change_directory(char *path, t_env **env, char *oldpwd, char *upgraded)
{
	char	*newpwd;

	if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		write(2, path, ft_strlen(path));
		ft_putstr_fd(" No such file or directory\n", 2);
		free(oldpwd);
		return (1);
	}
	newpwd = getcwd(NULL, 0);
	if (!oldpwd && !newpwd)
		handle_getcwd_error(&newpwd, env);
	if (oldpwd)
		updat_env(env, "OLDPWD", oldpwd);
	else
		updat_env(env, "OLDPWD", get_my_env("PWD", *env));
	updat_env(env, "PWD", newpwd);
	free(oldpwd);
	free(newpwd);
	free(upgraded);
	return (0);
}

int	ft_cd(char **args, t_env **env)
{
	char	*oldpwd;
	char	*path;
	char	*upgraded;

	if (args[1] && args[1][0] == '\0')
		return (0);
	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell : cd: too many arguments\n", 2);
		return (1);
	}
	oldpwd = getcwd(NULL, 0);
	path = resolve_cd_path(args, env, &upgraded);
	if (!path)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		return (free(oldpwd), 1);
	}
	if (!path[0])
		return (free(oldpwd), 0);
	return (change_directory(path, env, oldpwd, upgraded));
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
