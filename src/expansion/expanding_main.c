/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:09:56 by houabell          #+#    #+#             */
/*   Updated: 2025/06/21 18:25:01 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	has_variable(t_token *token)
{
	int	i;

	i = 0;
	while (token->value[i])
	{
		if (token->value[i] == '$' && is_allowed(token->value[i + 1]))
			return (1);
		i++;
	}
	return (0);
}

void	process_each_token(t_token **token, t_var_info **var, t_shell *shell, \
		int is_from_redir)
{
	char	**segments;
	t_token	*new_list;
	t_token	*first_to_free;

	segments = expand_token_value((*token)->value, var, shell);
	if (!segments || !segments[0])
		return ;
	new_list = create_token_list(segments, is_from_redir);
	if (!new_list)
		return ;
	first_to_free = new_list;
	replace_first_token(*token, new_list);
	insert_remaining_tokens(*token, new_list);
	free(first_to_free->value);
	free(first_to_free);
	free_arr(segments);
}

int	check_token_condition(t_token *cur, t_token *prev)
{
	int	is_redir_target;

	is_redir_target = 0;
	if (prev && is_redirection(prev->type))
		is_redir_target = 1;
	if (cur->is_heredoc_delimiter_value)
		return (-1);
	return (is_redir_target);
}

void	print_ambiguous_redirect_error(t_shell *shell, t_token *token)
{
	printf("minishell: %s: ambiguous redirect\n", token->value);
	shell->exit_status = 1;
	free_tokens(shell->tokens);
	shell->tokens = NULL;
	free_var_info_list(shell->variables);
	shell->variables = NULL;
}
