/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_single_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 21:31:17 by houabell          #+#    #+#             */
/*   Updated: 2025/06/17 11:15:34 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	populate_cmd_from_tokens(t_command *cmd, t_token **tokens)
{
	int	i;

	i = 0;
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if (is_redirection((*tokens)->type))
		{
			add_redirect(cmd, (*tokens)->type, (*tokens)->next->value);
			*tokens = (*tokens)->next->next;
		}
		else if ((*tokens)->type == TOKEN_WORD)
		{
			cmd->args[i++] = ft_strdup((*tokens)->value);
			*tokens = (*tokens)->next;
		}
	}
	cmd->args[i] = NULL;
}

static t_command	*init_command(t_token *tokens)
{
	t_command	*cmd;
	int			arg_count;
	int			redir_count;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->redirects = NULL;
	cmd->next = NULL;
	count_args_and_redirs(tokens, &arg_count, &redir_count);
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
		return (free(cmd), NULL);
	return (cmd);
}

t_command	*parse_single_command(t_token **tokens)
{
	t_command	*cmd;

	if (!*tokens)
		return (NULL);
	cmd = init_command(*tokens);
	if (!cmd)
		return (NULL);
	populate_cmd_from_tokens(cmd, tokens);
	return (cmd);
}
