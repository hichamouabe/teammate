/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 22:29:17 by houabell          #+#    #+#             */
/*   Updated: 2025/06/13 10:34:57 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static	int	get_and_process_input(t_shell *shell)
{
	char	*prompt;

	prompt = readline("minishell> ");
	if (!prompt)
	{
		printf("exit\n");
		shell->running = 0;
		return (ERROR);
	}
	if (*prompt)
	{
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
	handle_signals();
	while (shell->running)
	{
		if (get_and_process_input(shell) == ERROR)
			break ;
		if (shell->input)
		{
			process_command(shell);
			reset_shell(shell);
		}
	}
	return (shell->exit_status);
}
