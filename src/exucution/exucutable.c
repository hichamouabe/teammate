/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exucutable.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 10:48:33 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/22 16:33:56 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**env_to_array(t_env *env)
{
	t_env	*tmp;
	char	**envp;
	int		count;
	int		i;

	tmp = env;
	count = ft_lstsize(tmp);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	i = 0;
	if (env_to_array2(envp, &i, tmp) != 0)
		return (NULL);
	envp[i] = NULL;
	return (envp);
}

/*char	*find_executable(char *cmd, char *env_path)
{
	char	**dirs;
	char	*full_path;
	char	*tmp;
	int		i;

	dirs = ft_split(env_path, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		if (!tmp)
			return (free_arr(dirs), NULL);
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full_path)
			return (free_arr(dirs), NULL);
		if (access(full_path, X_OK) == 0)
			return (free_arr(dirs), full_path);
		i++;
		free(full_path);
	}
	return (free_arr(dirs), NULL);
}*/

char	*find_executable(char *cmd, char *env_path)
{
	char		**dirs;
	char		*full_path;
	char		*tmp;
	int			i;
	struct stat	path_stat; // Add a stat struct

	dirs = ft_split(env_path, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, F_OK) == 0) // Check for existence first
		{
			stat(full_path, &path_stat);
			if (!S_ISDIR(path_stat.st_mode)) // Check if it's NOT a directory
			{
				if (access(full_path, X_OK) == 0) // Then check for execute permission
					return (free_arr(dirs), full_path);
			}
		}
		free(full_path);
		i++;
	}
	free_arr(dirs);
	return (NULL);
}

/*int	check_executable(char *cmd, char *env_path, char **full_path)
{
	if (!cmd)
		return (127);
	if (cmd[0] == '/' || ft_strcmp(cmd, "../") == 0 \
		|| (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, F_OK) != 0)
			return (127);
		if (access(cmd, X_OK) == 0)
		{
			*full_path = ft_substr(cmd, 0, ft_strlen(cmd));
			if (!*full_path)
				return (1);
			return (0);
		}
		return (126);
	}
	if (env_path)
		*full_path = find_executable(cmd, env_path);
	else
		*full_path = ft_strjoin("./", cmd);
	if (!*full_path)
		return (127);
	return (0);
}*/
static int	check_direct_path(char *cmd, char **full_path)
{
	struct stat	path_stat;

	if (stat(cmd, &path_stat) != 0)
		return (127);
	if (S_ISDIR(path_stat.st_mode))
		return (126);
	if (access(cmd, X_OK) != 0)
		return (126);
	*full_path = ft_strdup(cmd);
	if (!*full_path)
		return (1);
	return (0);
}

int	check_executable(char *cmd, char *env_path, char **full_path)
{
	if (!cmd || cmd[0] == '\0')
		return (127);
	if (ft_strchr(cmd, '/'))
	{
		return (check_direct_path(cmd, full_path));
	}
	if (env_path)
		*full_path = find_executable(cmd, env_path);
	else
		*full_path = NULL;
	if (!*full_path)
		return (127);
	return (0);
}

/*int	check_status( char **args, char *env_path, char **full_path)
{
	int	status;

	if (!args || !args[0])
		return (1);
	status = check_executable(args[0], env_path, full_path);
	if (status == 127)
	{
		ft_putstr_fd(args[0], 2);
		if (env_path)
			ft_putstr_fd(" minishell : command not found\n", 2);
		else
			ft_putstr_fd(" minishell : No such file or directory\n", 2);
	}
	else if (status == 126)
		ft_putstr_fd("minishell : permission\n", 2);
	return (status);
}*/

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
			ft_putstr_fd(": command not found\n", 2);
		else if (status == 126)
		{
			if (stat(args[0], &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
				ft_putstr_fd(": is a directory\n", 2);
			else
				ft_putstr_fd(": Permission denied\n", 2);
		}
	}
	return (status);
}

int	is_not_builtin(char **args, char *env_path, t_env *env)
{
	char	*full_path;
	int		status;
	char	**envp;

	full_path = NULL;
	status = check_status(args, env_path, &full_path);
	if (status != 0)
		exit(status); // Exit the child process
	envp = env_to_array(env);
	if (!envp)
	{
		free(full_path);
		exit(1);
	}
	execve(full_path, args, envp);
	// If execve returns, it failed.
	free(full_path);
	free_arr(envp);
	perror(args[0]);
	exit(126); // Should exit child
}
