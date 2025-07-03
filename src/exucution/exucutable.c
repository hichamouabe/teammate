/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exucutable.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 10:48:33 by aelbouz           #+#    #+#             */
/*   Updated: 2025/07/02 09:16:01 by aelbouz          ###   ########.fr       */
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

char	*search_executable_in_dirs(char **dirs, char *cmd, int *i, \
		char **full_path)
{
	char		*tmp;
	struct stat	path_stat;

	while (dirs[*i])
	{
		tmp = ft_strjoin(dirs[*i], "/");
		*full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(*full_path, F_OK) == 0)
		{
			stat(*full_path, &path_stat);
			if (!S_ISDIR(path_stat.st_mode))
			{
				if (access(*full_path, X_OK) == 0)
					return (*full_path);
			}
		}
		free(*full_path);
		(*i)++;
	}
	return (NULL);
}

char	*find_executable(char *cmd, char *env_path)
{
	char	**dirs;
	char	*full_path;
	int		i;

	dirs = ft_split(env_path, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	full_path = NULL;
	if (search_executable_in_dirs(dirs, cmd, &i, &full_path))
		return (free_arr(dirs), full_path);
	return (free_arr(dirs), NULL);
}

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
		return (check_direct_path(cmd, full_path));
	if (env_path)
		*full_path = find_executable(cmd, env_path);
	else
		*full_path = NULL;
	if (!*full_path)
		return (127);
	return (0);
}
