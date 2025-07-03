/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 01:57:51 by houabell          #+#    #+#             */
/*   Updated: 2025/06/21 21:54:27 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_redirection_syntax(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (is_redirection(current->type))
		{
			if (!current->next || current->next->type != TOKEN_WORD)
			{
				printf("minishell: syntax error near unexpected token `%s'\n",
					current->value);
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}

int	check_pipe_syntax(t_token *tokens)
{
	t_token	*current;
	int		is_first;

	current = tokens;
	is_first = 1;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (is_first)
			{
				printf("minishell: syntax error near unexpected token '|'\n");
				return (0);
			}
			if (!current->next || current->next->type == TOKEN_PIPE)
			{
				printf("minishell: syntax error near unexpected token '|'\n");
				return (0);
			}
		}
		current = current->next;
		is_first = 0;
	}
	return (1);
}

int	check_ambig(t_shell *shell)
{
	t_token	*current;
	t_token	*prev;

	current = shell->tokens;
	prev = NULL;
	while (current)
	{
		if (prev && is_redirection(prev->type) && \
				prev->type != TOKEN_HEREDOC && current->value[0] == '\0')
		{
			printf("minishell: No such file or directory\n");
			shell->exit_status = 1;
			return (ERROR);
		}
		if (current->is_from_redir && current->next && \
					current->next->is_from_redir)
		{
			printf("minishell: ambiguous redirect\n");
			shell->exit_status = 1;
			return (ERROR);
		}
		prev = current;
		current = current->next;
	}
	return (SUCCESS);
}
