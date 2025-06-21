/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_words_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:58:46 by houabell          #+#    #+#             */
/*   Updated: 2025/06/21 17:16:59 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_quoted_part(t_quote_params *params, int expect_deli)
{
	char	*quoted_content;
	char	*new_word;

	quoted_content = NULL;
	if (append_segment_to_word(params->input, *(params->start), \
				*(params->i), params->word) == -1)
		return (-1);
	if (!handle_quotes(params->input, params->i, &quoted_content))
		return (-1);
	if (!expect_deli)
		log_expandable(params->shell, quoted_content);
	new_word = append_str(*(params->word), quoted_content);
	free(quoted_content);
	if (!new_word)
		return (-1);
	*(params->word) = new_word;
	*(params->start) = *(params->i);
	return (0);
}

int	handle_quote_in_word(char *input, int *i, int *start, \
		t_word_context *ctx)
{
	t_quote_params	params;

	if (input[*i] == '\'')
		ctx->shell->csqt = QUOTE_SINGLE;
	else
		ctx->shell->csqt = QUOTE_DOUBLE;
	params.input = input;
	params.i = i;
	params.start = start;
	params.word = &(ctx->word);
	params.shell = ctx->shell;
	if (handle_quoted_part(&params, (ctx->expect_delimiter)) == -1)
		return (free(ctx->word), -1);
	ctx->shell->csqt = QUOTE_NONE;
	return (0);
}

void	init_word_context(t_word_context *ctx, t_shell *shell)
{
	ctx->word = ft_strdup("");
	ctx->token = NULL;
	ctx->shell = shell;
}

t_token	*finalize_word(char *input, int start, int *i, \
		t_word_context *ctx)
{
	char	*var_i_think;
	t_token	*result;

	var_i_think = ft_strndup(input + start, *i - start);
	if (!ctx->expect_delimiter && var_i_think)
		log_expandable(ctx->shell, var_i_think);
	if (append_segment_to_word(input, start, *i, &(ctx->word)) == -1)
		return (free(ctx->word), free(var_i_think), NULL);
	result = new_token(TOKEN_WORD, ctx->word);
	return (free(ctx->word), free(var_i_think), result);
}

int	segment_has_any_actual_quotes(const char *segment_str, int len)
{
	int	k;

	k = 0;
	while (k < len)
	{
		if (segment_str[k] == '\'' || segment_str[k] == '"')
			return (1);
		k++;
	}
	return (0);
}
