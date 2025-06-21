/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_core.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 06:07:21 by houabell          #+#    #+#             */
/*   Updated: 2025/06/05 22:50:43 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*new_token(t_token_type type, char *value)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	if (!token->value && value != NULL)
	{
		free(token);
		return (NULL);
	}
	token->is_heredoc_delimiter_value = 0;
	token->original_delimiter_had_quotes = 0;
	token->is_from_redir = 0;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **tokens, t_token *new)
{
	t_token	*temp;

	if (!new)
		return ;
	if (!*tokens)
	{
		*tokens = new;
		return ;
	}
	temp = *tokens;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		if (temp->value)
			free(temp->value);
		free(temp);
	}
}

int	handle_quotes(char *input, int *i, char **value)
{
	char	quote;
	int		start;

	quote = input[*i];
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == '\0')
	{
		printf("ERROR: Unclosed quote: %c\n", quote);
		return (0);
	}
	*value = ft_strndup(&input[start], *i - start);
	if (!value)
		return (0);
	(*i)++;
	return (1);
}
