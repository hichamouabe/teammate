/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediractions3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:47:56 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/19 11:13:42 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	close_fds(t_command *cmd, int stdout_save, int stdin_save)
{
	if (cmd->file != -1)
		close(cmd->file);
	if (stdin_save != -1)
		close(stdin_save);
	if (stdout_save != -1)
		close(stdout_save);
}

char	*get_my_env(char *name, t_env *env)
{
	t_env	*tmp;

	if (!name || !env)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, name) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

static int	handle_redirects(t_command *cmd, int *file)
{
	if (cmd->redirects && (cmd->redirects->type == TOKEN_REDIR_OUT || \
		cmd->redirects->type == TOKEN_REDIR_APPEND) && \
		dup2(*file, STDOUT_FILENO) == -1)
		return (1);
	if (cmd->redirects && (cmd->redirects->type == TOKEN_REDIR_IN) \
		&& dup2(*file, STDIN_FILENO) == -1)
		return (1);
	return (0);
}

int	execute_with_setup(t_command **cmds, t_command *cmd, \
	t_execution_info *info, char *env_path)
{
	if (setup_io(info) != 0)
		return (1);
	if (info->i < info->cmd_count - 1 && pipe(cmd->fd) == -1)
		return (perror("minishell: pipe"), 1);
	if (fork() == 0)
	{
		if (info->i > 0)
			dup2(cmds[info->i - 1]->fd[0], STDIN_FILENO);
		if (info->i < info->cmd_count - 1)
			dup2(cmd->fd[1], STDOUT_FILENO);
		if (handle_redir(cmd) != 0)
			exit(1);
		if (cmd->file != -1)
		{
			if (handle_redirects(cmd, &cmd->file) != 0)
				exit(1);
		}
		if (!env_path)
			exit(1);
		info->status = is_builtin(cmd->args[0], cmd->args, env_path, info->env);
		exit(EXIT_SUCCESS);
	}
	waitpid(-1, &info->status, 0);
	return (0);
}

int	cleanup_execution(t_execution_info *info)
{
	if (info->stdout_save != -1)
		dup2(info->stdout_save, STDOUT_FILENO);
	if (info->stdin_save != -1)
		dup2(info->stdin_save, STDIN_FILENO);
	if (info->stdout_save != -1)
		close(info->stdout_save);
	if (info->stdin_save != -1)
		close(info->stdin_save);
	return (0);
}
