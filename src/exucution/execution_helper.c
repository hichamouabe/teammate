/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:31:00 by aelbouz           #+#    #+#             */
/*   Updated: 2025/07/02 15:08:49 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_child_process(t_pipeline_data *data)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (*data->in_fd != STDIN_FILENO)
	{	
		if (dup2(*data->in_fd, STDIN_FILENO) == -1)
			exit(1);
		close(*data->in_fd);
	}
	if (*data->i < data->cmd_count - 1)
	{
		close(data->pipe_fd[0]);
		if (dup2(data->pipe_fd[1], STDOUT_FILENO) == -1)
			exit(1);
		close(data->pipe_fd[1]);
	}
	if (!data->cmds[*data->i]->args || !data->cmds[*data->i]->args[0])
	{
		handle_redir(data->cmds[*data->i]);
		exit(0);
	}
	if (handle_redir(data->cmds[*data->i]) != 0)
		exit(1);
	exit(is_builtin(data->cmds[*data->i]->args[0], data->cmds[*data->i]->args, \
		&data->shell->env, 1));
}

void	process_pipeline_loop(t_pipeline_data *data)
{
	while (*data->i < data->cmd_count)
	{
		if (*data->i < data->cmd_count - 1 && pipe(data->pipe_fd) == -1)
			return (perror("minishell: pipe"));
		*data->pid = fork();
		if (*data->pid == -1)
			return (perror("minishell: fork"));
		if (*data->pid == 0)
			execute_child_process(data);
		if (*data->in_fd != STDIN_FILENO)
			close(*data->in_fd);
		if (*data->i < data->cmd_count - 1)
		{
			close(data->pipe_fd[1]);
			*data->in_fd = data->pipe_fd[0];
		}
		*data->last_pid = *data->pid;
		(*data->i)++;
	}
}

void	execute_pipeline(t_command **cmds, int cmd_count, t_shell *shell)
{
	int				i;
	int				in_fd;
	pid_t			pid;
	pid_t			last_pid;
	t_pipeline_data	data;

	i = 0;
	in_fd = STDIN_FILENO;
	data.i = &i;
	data.in_fd = &in_fd;
	data.pipe_fd[0] = -1;
	data.pipe_fd[1] = -1;
	data.pid = &pid;
	data.last_pid = &last_pid;
	data.cmds = cmds;
	data.cmd_count = cmd_count;
	data.shell = shell;
	signal(SIGINT, SIG_IGN);
	process_pipeline_loop(&data);
	wait_for_pipeline(*data.last_pid, cmd_count, shell);
	handle_signals();
}

void	prepare_command_array(t_command **cmds_array, int cmd_count, \
		t_command *cmd_list, t_shell *shell)
{
	int	i;

	i = 0;
	while (cmd_list)
	{
		cmds_array[i++] = cmd_list;
		cmd_list = cmd_list->next;
	}
	execute_pipeline(cmds_array, cmd_count, shell);
	free(cmds_array);
}

void	execute_commands(t_shell *shell)
{
	t_command	*cmd_list;
	t_command	**cmds_array;
	int			cmd_count;

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
		prepare_command_array(cmds_array, cmd_count, cmd_list, shell);
	}
}
