/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helper2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:48:59 by aelbouz           #+#    #+#             */
/*   Updated: 2025/07/02 14:16:16 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_status(char **args, char *env_path, char **full_path)
{
	int			status;
	struct stat	path_stat;

	status = check_executable(args[0], env_path, full_path);
	if (status != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		if (status == 127)
		{
			if (env_path)
				ft_putstr_fd(": command not found\n", 2);
			else if (env_path == NULL)
				ft_putstr_fd(": No such file or directory\n", 2);
		}
		else if (status == 126)
		{
			if (stat(args[0], &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
				ft_putstr_fd(": is a directory\n", 2);
			else
				ft_putstr_fd(": Permission denied\n", 2);
		}
	}
	return (get_exit_status(status, 1));
}

int	is_not_builtin(char **args, char *env_path, t_env *env)
{
	char	*full_path;
	int		status;
	char	**envp;

	full_path = NULL;
	status = check_status(args, env_path, &full_path);
	if (status != 0)
		return (status);
	envp = env_to_array(env);
	if (!envp)
	{
		free(full_path);
		exit(1);
	}
	if (execve(full_path, args, envp) == -1)
	{
		free(full_path);
		free_arr(envp);
		perror(args[0]);
		return (1);
	}
	return (0);
}

void	free_cmd(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->redirects)
		free_redirects(cmd->redirects);
	free(cmd);
}

int	get_exit_status(int status, int flag)
{
	static int	exit_status;

	if (flag == 1)
		exit_status = status;
	return (exit_status);
}

int	ft_exit(char **args, int in_pipe)
{
	if (in_pipe == 0)
		ft_putstr_fd("exit\n", 1);
	if (!args || !args[1])
		exit (get_exit_status(0, 0));
	if ((is_numeric(args[1])) == 1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": exit: numeric argument required\n", 2);
		exit (2);
	}
	else if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		exit (1);
	}
	exit (ft_atoi(args[1]) % 256);
}
