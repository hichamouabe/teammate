/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_words.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:11:05 by houabell          #+#    #+#             */
/*   Updated: 2025/06/05 20:27:51 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	prepare_unquoted_segment(char *input, int start_seg, \
		int *i, t_word_context *ctx)
{
	char	*unquoted_seg;

	unquoted_seg = ft_strndup(input + start_seg, *i - start_seg);
	if (!unquoted_seg)
	{
		free(ctx->word);
		return (-1);
	}
	if (!ctx->expect_delimiter)
	{
		ctx->shell->csqt = QUOTE_NONE;
		log_expandable(ctx->shell, unquoted_seg);
	}
	free(unquoted_seg);
	return (0);
}

static int	process_word_loop(char *input, int *i, int *start_seg, \
		t_word_context *ctx)
{
	while (input[*i] && !is_whitespace(input[*i]) && !is_special(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			if (*i > *start_seg)
				if (prepare_unquoted_segment(input, *start_seg, i, ctx) == -1)
					return (-1);
			if (handle_quote_in_word(input, i, start_seg, ctx) == -1)
				return (-1);
		}
		else
			(*i)++;
	}
	return (0);
}

static t_token	*finish_word_token(t_word_state *state, t_word_context *ctx)
{
	t_token	*final_token;

	ctx->shell->csqt = QUOTE_NONE;
	final_token = finalize_word(state->input, state->start_seg, state->i, ctx);
	if (final_token && ctx->expect_delimiter)
	{
		final_token->is_heredoc_delimiter_value = 1;
		final_token->original_delimiter_had_quotes \
			= segment_has_any_actual_quotes(state->input + state->start_word, \
					*(state->i) - state->start_word);
	}
	return (final_token);
}

t_token	*handle_word(char *input, int *i, t_shell *shell, int expect_delimiter)
{
	t_word_context	ctx;
	t_word_state	state;

	init_word_context(&ctx, shell);
	ctx.expect_delimiter = expect_delimiter;
	if (!ctx.word)
		return (NULL);
	state.input = input;
	state.start_word = *i;
	state.start_seg = *i;
	state.i = i;
	if (process_word_loop(input, i, &state.start_seg, &ctx) == -1)
		return (NULL);
	return (finish_word_token(&state, &ctx));
}
