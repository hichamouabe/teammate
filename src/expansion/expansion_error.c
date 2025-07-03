/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 00:35:45 by houabell          #+#    #+#             */
/*   Updated: 2025/06/02 03:25:58 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	cleanup_expansion_step(char *var_name, char *expanded_value, \
		t_var_info **var_list)
{
	free(var_name);
	free(expanded_value);
	*var_list = (*var_list)->next;
}

void	handle_extraction_failure(t_expansion_state *state, t_var_info \
		**var_list)
{
	if (state->buffer && *state->buffer)
		add_segment(&state->result, ft_strdup(state->buffer));
	printf("allocation error durring the variable extraction\n");
	printf("skipp from %s --To--> %s\n", \
			(*var_list)->name, (*var_list)->next->name);
	free(state->buffer);
	state->buffer = NULL;
}

void	handle_expansion_failure(t_expansion_state *state, char *var_name)
{
	free(var_name);
	if (state->buffer && *state->buffer)
		add_segment(&state->result, ft_strdup(state->buffer));
	printf("allocation error durring the expansion\n");
	free(state->buffer);
	state->buffer = NULL;
}
