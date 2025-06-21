/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 22:14:52 by houabell          #+#    #+#             */
/*   Updated: 2025/06/21 08:32:17 by aelbouz          ###   ########.fr       */
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

void	reset_shell(t_shell *shell)
{
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->commands)
	{
		free_cmd(shell->commands);
		shell->commands = NULL;
	}
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	if (shell->variables)
	{
		free_var_info_list(shell->variables);
		shell->variables = NULL;
	}
	if (shell->heredoc_files)
		cleanup_files(shell);
	shell->heredoc_sigint = 0;
}
