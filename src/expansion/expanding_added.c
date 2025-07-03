/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_added.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 17:50:39 by houabell          #+#    #+#             */
/*   Updated: 2025/06/21 18:33:54 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	replace_token_with_segments(t_token **p_cur, char **segments, \
		int is_redir_target)
{
	t_token	*new_list;
	t_token	*last_new;
	t_token	*original_next;

	new_list = create_token_list(segments, is_redir_target);
	if (!new_list && (free_arr(segments), 1))
		return ;
	original_next = (*p_cur)->next;
	last_new = get_last_token(new_list);
	last_new->next = original_next;
	free((*p_cur)->value);
	free(*p_cur);
	*p_cur = new_list;
	free_arr(segments);
}

static void	remove_token(t_token **p_cur)
{
	t_token	*to_free;

	to_free = *p_cur;
	*p_cur = (*p_cur)->next;
	free(to_free->value);
	free(to_free);
}

static void	process_token_expansion(t_token **p_cur, t_token *prev, \
		t_shell *shell, t_var_info **vars_cursor)
{
	char	**segments;
	int		segment_count;
	int		is_redir_target;

	is_redir_target = check_token_condition(*p_cur, prev);
	segments = expand_token_value((*p_cur)->value, vars_cursor, shell);
	segment_count = 0;
	if (segments)
		while (segments[segment_count])
			segment_count++;
	if (is_redir_target && segment_count != 1)
	{
		print_ambiguous_redirect_error(shell, *p_cur);
		free_arr(segments);
		return ;
	}
	if (!segments)
		remove_token(p_cur);
	else
		replace_token_with_segments(p_cur, segments, is_redir_target);
}

void	expand_variables(t_shell *shell)
{
	t_token		**p_cur;
	t_token		*prev;
	t_var_info	*vars_cursor;
	int			is_expandable;

	p_cur = &shell->tokens;
	prev = NULL;
	vars_cursor = shell->variables;
	while (*p_cur)
	{
		is_expandable = ((*p_cur)->type == TOKEN_WORD && has_variable(*p_cur));
		if (check_token_condition(*p_cur, prev) != -1 && is_expandable)
		{
			process_token_expansion(p_cur, prev, shell, &vars_cursor);
			if (shell->tokens == NULL)
				break ;
		}
		if (*p_cur)
		{
			prev = *p_cur;
			p_cur = &(*p_cur)->next;
		}
	}
}
