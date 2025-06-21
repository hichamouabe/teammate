/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 21:19:46 by houabell          #+#    #+#             */
/*   Updated: 2025/06/13 10:37:25 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	create_commands(t_shell *shell)
{
	t_token		*token_ptr;
	t_command	*new_cmd;
	t_command	*last_cmd;

	token_ptr = shell->tokens;
	shell->commands = NULL;
	last_cmd = NULL;
	while (token_ptr != NULL)
	{
		new_cmd = parse_single_command(&token_ptr);
		if (!new_cmd)
		{
			free_cmd(shell->commands);
			shell->commands = NULL;
		}
		if (last_cmd == NULL)
			shell->commands = new_cmd;
		else
			last_cmd->next = new_cmd;
		last_cmd = new_cmd;
		if (token_ptr && token_ptr->type == TOKEN_PIPE)
			token_ptr = token_ptr->next;
	}
}
