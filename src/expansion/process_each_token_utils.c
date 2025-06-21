/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_each_token_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 00:14:44 by houabell          #+#    #+#             */
/*   Updated: 2025/06/06 01:08:35 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	mark_tokens(t_token *token_list)
{
	t_token	*curr;

	curr = token_list;
	while (curr)
	{
		curr->is_from_redir = 1;
		curr = curr->next;
	}
}

t_token	*create_token_list(char **segments, int is_from_redir)
{
	t_token	*new_list;
	t_token	*new;
	int		i;

	new_list = NULL;
	i = 0;
	while (segments[i])
	{
		new = new_token(TOKEN_WORD, segments[i]);
		if (!new)
		{
			free_tokens(new_list);
			return (NULL);
		}
		add_token(&new_list, new);
		i++;
	}
	if (is_from_redir)
		mark_tokens(new_list);
	return (new_list);
}

t_token	*get_last_token(t_token *token_list)
{
	t_token	*last;

	last = token_list;
	while (last->next)
		last = last->next;
	return (last);
}

void	replace_first_token(t_token *original, t_token *new_first)
{
	free(original->value);
	original->type = new_first->type;
	original->value = ft_strdup(new_first->value);
	original->is_from_redir = new_first->is_from_redir;
}

void	insert_remaining_tokens(t_token *original, t_token *new_list)
{
	t_token	*remaining_tokens;
	t_token	*last_new;
	t_token	*original_next;

	if (!new_list->next)
		return ;
	remaining_tokens = new_list->next;
	original_next = original->next;
	original->next = remaining_tokens;
	last_new = get_last_token(remaining_tokens);
	last_new->next = original_next;
}
