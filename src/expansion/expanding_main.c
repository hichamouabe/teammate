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

/*static void	move_to_next_token(t_token **cur_token, t_token **prev_token)
{
	*prev_token = *cur_token;
	if (*cur_token)
		*cur_token = (*cur_token)->next;
}*/

void	print_ambiguous_redirect_error(t_shell *shell, t_token *token)
{
	printf("minishell: %s: ambiguous redirect\n", token->value);
	shell->exit_status = 1;
	free_tokens(shell->tokens);
	shell->tokens = NULL;
	free_var_info_list(shell->variables);
	shell->variables = NULL;
}

/*void	expand_variables(t_shell *shell)
{
	t_token		*cur;
	t_token		*prev;
	t_token		*next_node;
	t_var_info	*vars;
	int			is_redir_target;
	char		**segments;
	int			segment_count;

	cur = shell->tokens;
	prev = NULL;
	vars = shell->variables;
	while (cur != NULL)
	{
		next_node = cur->next;
		is_redir_target = check_token_condition(cur, prev);
		if (is_redir_target == -1 || !(cur->type == TOKEN_WORD && has_variable(cur)))
		{
			prev = cur;
			cur = next_node;
			continue ;
		}
		segments = expand_token_value(cur->value, &vars, shell);
		segment_count = 0;
		if (segments)
			while (segments[segment_count])
				segment_count++;
		
		if (is_redir_target && segment_count != 1)
		{
			print_ambiguous_redirect_error(shell, cur);
			free_arr(segments);
			return ;
		}
		
		if (!segments) // This block is now only for non-redirect expansions
		{
			if (prev)
				prev->next = next_node;
			else
				shell->tokens = next_node;
			free(cur->value);
			free(cur);
			cur = next_node;
		}
		else
		{
			t_token *new_list;
			t_token *temp_for_free;

			new_list = create_token_list(segments, is_redir_target);
			if (new_list)
			{
				temp_for_free = new_list;
				replace_first_token(cur, new_list);
				insert_remaining_tokens(cur, new_list);
				free(temp_for_free->value);
				free(temp_for_free);
			}
			free_arr(segments);
			prev = cur;
			cur = next_node;
		}
	}
}*/
