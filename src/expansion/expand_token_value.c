/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_value.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:28:50 by houabell          #+#    #+#             */
/*   Updated: 2025/06/21 17:17:50 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**finalize_expansion(t_expansion_state *state)
{
	if (state->buffer)
		add_segment(&state->result, state->buffer);
	return (state->result);
}

int	is_variable_start(char *str, int pos, t_var_info **var_list)
{
	return (str[pos] == '$' && is_allowed(str[pos + 1]) \
			&& var_list && *var_list);
}

void	init_expansion_state(t_expansion_state *state)
{
	state->result = NULL;
	state->buffer = NULL;
	state->i = 0;
}

char	**expand_token_value(char *str, t_var_info **var_list, t_shell *shell)
{
	t_expansion_state	state;

	init_expansion_state(&state);
	while (str[state.i])
	{
		if (is_variable_start(str, state.i, var_list))
			process_variable_expansion(str, &state, var_list, shell);
		else
		{
			if (str[state.i] == '$')
			{
				append_segment(&state.buffer, "$");
				state.i++;
			}
			else
				process_regular_text(str, &state);
		}
	}
	return (finalize_expansion(&state));
}
