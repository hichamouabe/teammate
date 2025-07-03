/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 15:05:10 by houabell          #+#    #+#             */
/*   Updated: 2025/07/02 22:59:39 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0 && cmd[1] != '\0')
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	execute_single_command1(t_command *cmd, t_env **env, \
	int *stdout_save, int *stdin_save)
{
	int	status;

	if (!cmd->args || (!cmd->args[0] && !cmd->redirects))
		return (0);
	if (cmd->args && cmd->args[0] && is_parent_builtin(cmd->args[0]) == 1)
	{
		*stdout_save = dup(STDOUT_FILENO);
		*stdin_save = dup(STDIN_FILENO);
		if (handle_redir(cmd) != 0)
			return (close(*stdout_save), close(*stdin_save), 1);
		status = is_builtin(cmd->args[0], cmd->args, env, 0);
		dup2(*stdout_save, STDOUT_FILENO);
		dup2(*stdin_save, STDIN_FILENO);
		close(*stdout_save);
		close(*stdin_save);
		return (get_exit_status(status, 1));
	}
	return (-1);
}

int	execute_single_command2(pid_t pid, int *status)
{
	waitpid(pid, status, 0);
	handle_signals();
	if (WIFSIGNALED(*status))
	{
		if (WTERMSIG(*status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(*status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		return (128 + WTERMSIG(*status));
	}
	if (WIFEXITED(*status))
		return (WEXITSTATUS(*status));
	return (*status);
}

int	execute_single_command(t_command *cmd, t_env **env)
{
	int		status;
	int		stdout_save;
	int		stdin_save;
	pid_t	pid;

	status = execute_single_command1(cmd, env, &stdout_save, &stdin_save);
	if (status != -1)
		return (status);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), 1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (handle_redir(cmd) != 0)
			exit(1);
		exit(is_builtin(cmd->args[0], cmd->args, env, 0));
	}
	status = execute_single_command2(pid, &status);
	return (get_exit_status(status, 1));
}

/*void	wait_for_pipeline(pid_t last_pid, int cmd_count, t_shell *shell)
{
	int	status;
	int	i;

	if (last_pid > 0)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				printf("\n");
			if (WTERMSIG(status) == SIGQUIT)
				printf("Quit (core dumped)\n");
			shell->exit_status = 128 + WTERMSIG(status);
		}
	}
	get_exit_status(shell->exit_status, 1);
	i = 0;
	while (i < cmd_count)
	{
		if (wait(NULL) == -1 && errno == ECHILD)
			break ;
		i++;
	}
}*/

// in src/exucution/execution_main.c

void	wait_for_pipeline(pid_t last_pid, int cmd_count, t_shell *shell)
{
	int	status;
	int	i;
	int	newline_printed;

	newline_printed = 0;
	if (last_pid > 0)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
			{
				printf("\n");
				newline_printed = 1;
			}
			if (WTERMSIG(status) == SIGQUIT)
				printf("Quit (core dumped)\n");
			shell->exit_status = 128 + WTERMSIG(status);
		}
	}
	get_exit_status(shell->exit_status, 1);
	i = 0;
	while (i < cmd_count)
	{
		if (wait(&status) == -1 && errno == ECHILD)
			break ;
		if (!newline_printed && WIFSIGNALED(status)
			&& WTERMSIG(status) == SIGINT)
		{
			printf("\n");
			newline_printed = 1;
		}
		i++;
	}
}
