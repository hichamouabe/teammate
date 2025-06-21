/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_context.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 21:55:08 by houabell          #+#    #+#             */
/*   Updated: 2025/06/21 17:27:43 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	apply_expansion_by_context(t_expansion_state *state, \
		char *expanded_value, t_var_info **var_list)
{
	if ((*var_list)->quote_context == QUOTE_SINGLE)
		handle_single_quote_context(state, expanded_value, var_list);
	else if ((*var_list)->quote_context == QUOTE_DOUBLE)
		handle_double_quote_context(state, expanded_value);
	else
		handle_unquoted_context(state, expanded_value);
}

void	handle_single_quote_context(t_expansion_state *state, \
		char *expanded_value, t_var_info **var_list)
{
	(void)expanded_value;
	append_segment(&state->buffer, "$");
	append_segment(&state->buffer, (*var_list)->name);
}

void	handle_double_quote_context(t_expansion_state *state, \
		char *expanded_value)
{
	if (expanded_value)
		append_segment(&state->buffer, expanded_value);
}

void	handle_unquoted_context(t_expansion_state *state, char *expanded_value)
{
	if (!expanded_value)
		return ;
	if (!*expanded_value && !(state->buffer && *(state->buffer)))
	{
		return ;
	}
	process_word_splitting(state, expanded_value);
}
