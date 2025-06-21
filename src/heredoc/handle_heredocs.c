/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredocs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:14:36 by houabell          #+#    #+#             */
/*   Updated: 2025/06/21 17:39:14 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	count_heredocs(t_shell *shell)
{
	t_token	*current;
	int		count;

	current = shell->tokens;
	count = 0;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
			count++;
		current = current->next;
	}
	return (count);
}

static int	check_heredoc_limit(int count)
{
	if (count > MAX_HEREDOCS)
	{
		printf("minishell: maximum here-document count exceeded\n");
		return (ERROR);
	}
	return (SUCCESS);
}

static int	process_heredoc_token(t_token *token, t_shell *shell)
{
	int	quotes_flag;

	quotes_flag = token->next->original_delimiter_had_quotes;
	if (read_heredoc_input(token->next->value, !quotes_flag, shell) \
			!= SUCCESS)
		return (ERROR);
	token->type = TOKEN_REDIR_IN;
	free(token->value);
	token->value = ft_strdup("<");
	free(token->next->value);
	token->next->value \
		= ft_strdup(shell->heredoc_files[shell->heredoc_count - 1]);
	return (SUCCESS);
}

static int	process_all_heredocs(t_shell *shell)
{
	t_token	*current;

	current = shell->tokens;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
		{
			if (process_heredoc_token(current, shell) != SUCCESS)
				return (ERROR);
		}
		current = current->next;
	}
	return (SUCCESS);
}

int	handle_heredocs(t_shell *shell)
{
	int	count;

	count = count_heredocs(shell);
	if (check_heredoc_limit(count) != SUCCESS)
		return (ERROR);
	return (process_all_heredocs(shell));
}
