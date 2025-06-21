/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 21:46:39 by houabell          #+#    #+#             */
/*   Updated: 2025/06/21 21:42:36 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	count_args_and_redirs(t_token *start, int *arg_c, int *redir_c)
{
	*arg_c = 0;
	*redir_c = 0;
	while (start && start->type != TOKEN_PIPE)
	{
		if (start->type == TOKEN_WORD)
			(*arg_c)++;
		else if (is_redirection(start->type))
		{
			(*redir_c)++;
			start = start->next;
		}
		if (start)
			start = start->next;
	}
}

void	add_redirect(t_command *cmd, t_token_type type, char *filename)
{
	t_redirect	*new_redir;
	t_redirect	*current;

	new_redir = malloc(sizeof(t_redirect));
	if (!new_redir)
		return ;
	new_redir->type = type;
	new_redir->file = ft_strdup(filename);
	new_redir->next = NULL;
	if (!cmd->redirects)
		cmd->redirects = new_redir;
	else
	{
		current = cmd->redirects;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
}
