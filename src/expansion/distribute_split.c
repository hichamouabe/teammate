/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   distribute_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:48:40 by houabell          #+#    #+#             */
/*   Updated: 2025/06/13 09:53:58 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*prepare_first_word(t_expansion_state *state, char *first_split)
{
	char	*first_word_processed;

	if (state->buffer && *(state->buffer))
	{
		first_word_processed = ft_strjoin(state->buffer, first_split);
		free(state->buffer);
		state->buffer = NULL;
		return (first_word_processed);
	}
	else
	{
		first_word_processed = ft_strdup(first_split);
		return (first_word_processed);
	}
}

static void	handle_single_word(t_expansion_state *state, \
		char *first_word_processed)
{
	if (state->buffer)
		free(state->buffer);
	state->buffer = first_word_processed;
}

static void	handle_multiple_words(t_expansion_state *state, \
		char *first_word_processed)
{
	add_segment(&state->result, first_word_processed);
}

static void	process_remaining_words(t_expansion_state *state, char **split)
{
	int	j;

	j = 1;
	while (split[j])
	{
		if (split[j + 1] == NULL)
		{
			if (state->buffer)
				free(state->buffer);
			state->buffer = ft_strdup(split[j]);
			if (!state->buffer && split[j] && *split[j])
				return ;
		}
		else
			add_segment(&state->result, ft_strdup(split[j]));
		j++;
	}
}

void	distribute_split_words(t_expansion_state *state, char **split)
{
	char	*first_word_processed;

	if (!split || !split[0] || !*(split[0]))
	{
		if (split)
			free_arr(split);
		return ;
	}
	first_word_processed = prepare_first_word(state, split[0]);
	if (!first_word_processed)
	{
		free_arr(split);
		return ;
	}
	if (split[1] == NULL)
		handle_single_word(state, first_word_processed);
	else
		handle_multiple_words(state, first_word_processed);
	process_remaining_words(state, split);
	free_arr(split);
}
