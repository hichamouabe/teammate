/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 15:05:10 by houabell          #+#    #+#             */
/*   Updated: 2025/06/22 17:45:31 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_parent_builtin(char *cmd)
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

/*int	execute_single_command(t_command *cmd, t_env **env) // REMOVED t_shell *shell
{
	int		status;
	int		stdout_save;
	int		stdin_save;
	pid_t	pid;
	char	*env_path;

	if (!cmd->args || !cmd->args[0])
		return (0);
	if (is_parent_builtin(cmd->args[0]))
	{
		stdout_save = dup(STDOUT_FILENO);
		stdin_save = dup(STDIN_FILENO);
		if (handle_redir(cmd) != 0)
			return (dup2(stdout_save, STDOUT_FILENO), dup2(stdin_save, STDIN_FILENO),
				close(stdout_save), close(stdin_save), 1);
		status = is_builtin(cmd->args[0], cmd->args, NULL, env);
		dup2(stdout_save, STDOUT_FILENO);
		dup2(stdin_save, STDIN_FILENO);
		close(stdout_save);
		close(stdin_save);
		return (status);
	}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), 1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (handle_redir(cmd) != 0)
			exit(1);
		env_path = get_my_env("PATH", *env); // CHANGED from shell->env
		exit(is_builtin(cmd->args[0], cmd->args, env_path, env));
	}
	waitpid(pid, &status, 0);
	handle_signals();
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			printf("\n");
		else if (WTERMSIG(status) == SIGQUIT)
			printf("Quit (core dumped)\n");
		return (128 + WTERMSIG(status));
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (status);
}*/

int	execute_single_command(t_command *cmd, t_env **env)
{
	int		status;
	int		stdout_save;
	int		stdin_save;
	pid_t	pid;
	char	*env_path;

	if (!cmd->args || !cmd->args[0])
		return (0);
	if (is_parent_builtin(cmd->args[0]))
	{
		stdout_save = dup(STDOUT_FILENO);
		stdin_save = dup(STDIN_FILENO);
		if (handle_redir(cmd) != 0)
			return (dup2(stdout_save, STDOUT_FILENO), dup2(stdin_save, STDIN_FILENO),
				close(stdout_save), close(stdin_save), 1);
		status = is_builtin(cmd->args[0], cmd->args, NULL, env);
		dup2(stdout_save, STDOUT_FILENO);
		dup2(stdin_save, STDIN_FILENO);
		close(stdout_save);
		close(stdin_save);
		return (status);
	}

	// Temporarily ignore signals in the parent
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), 1);

	if (pid == 0) // Child process
	{
		// Child process should have default signal behavior
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (handle_redir(cmd) != 0)
			exit(1);
		env_path = get_my_env("PATH", *env);
		// is_builtin will call execve which replaces the process,
		// or it will run a builtin and exit.
		exit(is_builtin(cmd->args[0], cmd->args, env_path, env));
	}

	// Parent process waits for the child
	waitpid(pid, &status, 0);

	// Restore the parent's interactive signal handlers
	handle_signals();

	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		return (128 + WTERMSIG(status));
	}

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));

	return (status);
}

static void	wait_for_pipeline(pid_t last_pid, int cmd_count, t_shell *shell)
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
	i = 0;
	while (i < cmd_count)
	{
		if (wait(NULL) == -1 && errno == ECHILD)
			break ;
		i++;
	}
}

static void	execute_pipeline(t_command **cmds, int cmd_count, t_shell *shell)
{
	int		i;
	int		in_fd;
	int		pipe_fd[2];
	pid_t	pid;
	pid_t	last_pid;

	i = 0;
	in_fd = STDIN_FILENO;
	last_pid = -1;
	signal(SIGINT, SIG_IGN); // Parent ignores signals during pipeline
	while (i < cmd_count)
	{
		if (i < cmd_count - 1 && pipe(pipe_fd) == -1)
			return (perror("minishell: pipe"));
		pid = fork();
		if (pid == -1)
			return (perror("minishell: fork"));
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL); // Child handles signals by default
			signal(SIGQUIT, SIG_DFL);
			if (in_fd != STDIN_FILENO)
			{
				if (dup2(in_fd, STDIN_FILENO) == -1)
					exit(1);
				close(in_fd);
			}
			if (i < cmd_count - 1)
			{
				close(pipe_fd[0]);
				if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
					exit(1);
				close(pipe_fd[1]);
			}
			if (handle_redir(cmds[i]) != 0)
				exit(1);
			exit(is_builtin(cmds[i]->args[0], cmds[i]->args,
					get_my_env("PATH", shell->env), &shell->env));
		}
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (i < cmd_count - 1)
		{
			close(pipe_fd[1]);
			in_fd = pipe_fd[0];
		}
		last_pid = pid;
		i++;
	}
	wait_for_pipeline(last_pid, cmd_count, shell);
	handle_signals(); // Restore interactive handlers after all children are done
}

void	execute_commands(t_shell *shell)
{
	t_command	*cmd_list;
	t_command	**cmds_array;
	int			cmd_count;
	int			i;

	if (!shell || !shell->commands)
		return ;
	cmd_list = shell->commands;
	cmd_count = 0;
	while (cmd_list)
	{
		cmd_count++;
		cmd_list = cmd_list->next;
	}
	cmd_list = shell->commands;
	if (cmd_count == 1)
		shell->exit_status = execute_single_command(cmd_list, &shell->env);
	else if (cmd_count > 1)
	{
		cmds_array = malloc(sizeof(t_command *) * cmd_count);
		if (!cmds_array)
			return ;
		i = 0;
		while (cmd_list)
		{
			cmds_array[i++] = cmd_list;
			cmd_list = cmd_list->next;
		}
		execute_pipeline(cmds_array, cmd_count, shell);
		free(cmds_array);
	}
}
