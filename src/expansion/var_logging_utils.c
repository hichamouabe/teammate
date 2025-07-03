/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_logging_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 09:48:16 by houabell          #+#    #+#             */
/*   Updated: 2025/05/19 10:03:03 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_var_info	*create_var_node(char *name, t_quote_type context_csqt)
{
	t_var_info	*node;

	node = (t_var_info *)malloc(sizeof(t_var_info));
	if (!node)
		return (NULL);
	node->name = ft_strdup(name);
	if (!node->name && name != NULL)
		return (free(node), NULL);
	if (name)
		node->length = ft_strlen(name);
	else
		node->length = 0;
	node->quote_context = context_csqt;
	node->next = NULL;
	return (node);
}

void	add_var_node(t_var_info **var_head, t_var_info *new_node)
{
	t_var_info	*current;

	if (!new_node)
		return ;
	if (!*var_head)
		*var_head = new_node;
	else
	{
		current = *var_head;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

void	free_var(t_var_info *var_head)
{
	t_var_info	*current;
	t_var_info	*next_node;

	current = var_head;
	while (current)
	{
		next_node = current->next;
		if (current->name)
			free(current->name);
		free(current);
		current = next_node;
	}
}
