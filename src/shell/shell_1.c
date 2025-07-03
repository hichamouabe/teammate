/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 22:14:52 by houabell          #+#    #+#             */
/*   Updated: 2025/07/03 04:19:32 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->env = init_env(envp);
	if (!shell->env)
		shell->env = init_default_env(&shell->env);
	shell->exit_status = 0;
	shell->running = 1;
	shell->heredoc_sigint = 0;
	shell->input = NULL;
	shell->variables = NULL;
	shell->heredoc_files = NULL;
	shell->heredoc_count = 0;
	shell->interactive = isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
	return (shell);
}

int	validate_syntax(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (!check_pipe_syntax(tokens))
		return (0);
	if (!check_redirection_syntax(tokens))
		return (0);
	return (1);
}

void	reset_shell_part1(t_shell *shell)
{
	t_command	*current_cmd;
	t_command	*next_cmd;

	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	current_cmd = shell->commands;
	while (current_cmd)
	{
		next_cmd = current_cmd->next;
		free_cmd(current_cmd);
		current_cmd = next_cmd;
	}
	shell->commands = NULL;
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
}

void	reset_shell_part2(t_shell *shell)
{
	if (shell->variables)
	{
		free_var_info_list(shell->variables);
		shell->variables = NULL;
	}
	if (shell->heredoc_files)
		cleanup_files(shell);
	shell->heredoc_sigint = 0;
}

void	reset_shell(t_shell *shell)
{
	reset_shell_part1(shell);
	reset_shell_part2(shell);
}
