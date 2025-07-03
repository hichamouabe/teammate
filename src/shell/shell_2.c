/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 22:29:17 by houabell          #+#    #+#             */
/*   Updated: 2025/07/03 04:05:35 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static	int	get_and_process_input(t_shell *shell)
{
	char	*prompt;

	if (shell->interactive)
		prompt = readline("minishell> ");
	else
		prompt = readline(NULL);
	if (!prompt)
	{
		if (shell->interactive)
			printf("exit\n");
		shell->running = 0;
		return (ERROR);
	}
	if (*prompt)
	{
		if (shell->interactive)
			add_history(prompt);
		shell->input = ft_strdup(prompt);
		free(prompt);
		if (!shell->input)
			return (ERROR);
		shell->tokens = tokenize(shell->input, shell);
		return (SUCCESS);
	}
	free(prompt);
	return (SUCCESS);
}

static int	process_command(t_shell *shell)
{
	if (shell->tokens)
	{
		if (validate_syntax(shell->tokens))
		{
			if (parse_tokens(shell))
				execute_commands(shell);
		}
		else
			shell->exit_status = get_exit_status(2, 1);
	}
	return (SUCCESS);
}

void	free_var_info_list(t_var_info *var_head)
{
	t_var_info	*current;
	t_var_info	*next_node;

	current = var_head;
	while (current)
	{
		next_node = current->next;
		if (current->name)
			free(current->name);
		free(current);
		current = next_node;
	}
}

int	minishell_loop(t_shell *shell)
{
	if (shell->interactive)
		handle_signals();
	while (shell->running)
	{
		if (get_and_process_input(shell) == ERROR)
			break ;
		if (g_signal_status != 0)
		{
			shell->exit_status = g_signal_status;
			g_signal_status = 0;
		}
		if (shell->input)
		{
			process_command(shell);
		}
		reset_shell(shell);
	}
	if (g_signal_status != 0)
		shell->exit_status = g_signal_status;
	return (shell->exit_status);
}

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->commands)
		free_cmd(shell->commands);
	if (shell->env)
		free_env(shell->env);
	if (shell->input)
		free(shell->input);
	if (shell->variables)
		free_var_info_list(shell->variables);
	if (shell->heredoc_files)
		cleanup_files(shell);
	free(shell);
}
