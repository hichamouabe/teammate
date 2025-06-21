/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_x.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 00:52:10 by houabell          #+#    #+#             */
/*   Updated: 2025/06/02 02:12:40 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_special(char c)
{
	return (c == '|' || c == '>' || c == '<');
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC);
}

int	ft_isalnum(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	append_segment_to_word(char *input, int start, \
		int current_i, char **word)
{
	char	*part;
	char	*new_word;

	if (current_i > start)
	{
		part = ft_strndup(&input[start], current_i - start);
		if (!part)
			return (-1);
		new_word = append_str(*word, part);
		free(part);
		if (!new_word)
			return (-1);
		*word = new_word;
	}
	return (0);
}
