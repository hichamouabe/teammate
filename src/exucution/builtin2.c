/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:21:49 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/21 08:33:35 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_n_flag(char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (1);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

int	ft_echo(char **args)
{
	int	no_new_line;
	int	i;

	if (!args)
		return (1);
	i = 1;
	no_new_line = 1;
	while (args[i] && is_n_flag(args[i]) == 0)
	{
		no_new_line = 0;
		i++;
	}
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (no_new_line == 1)
		write(1, "\n", 1);
	return (0);
}

int	ft_env(t_env *envp)
{
	while (envp)
	{
		if (envp->value)
			printf("%s=", envp->key);
		else if (envp->key[ft_strlen(envp->key) - 1] == '=')
			printf("%s\n", envp->key);
		if (envp->value)
			printf("%s\n", envp->value);
		envp = envp->next;
	}
	return (0);
}

int	is_builtin(char *cmd, char **args, char *env_path, t_env **env)
{
	if (!cmd)
		return (0);
	if (args[0][0] == '/')
		return (write(2, args[0], ft_strlen(args[0])), \
		ft_putstr_fd(" : is a directory\n", 2), 126);
	if (ft_strcmp(cmd, "echo") == 0)
		return (ft_echo(args));
	if (ft_strcmp(cmd, "pwd") == 0)
		return (ft_pwd(env));
	if (ft_strcmp(cmd, "exit") == 0)
		return (ft_exit(args));
	if (ft_strcmp(cmd, "cd") == 0)
		return (ft_cd(args, env));
	if (ft_strcmp(cmd, "env") == 0)
		return (ft_env(*env));
	if (ft_strcmp(cmd, "export") == 0)
		return (ft_export(args, env));
	if (ft_strcmp(cmd, "unset") == 0)
		return (ft_unset(args, env));
	return (is_not_builtin(args, env_path, *env));
}
