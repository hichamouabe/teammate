/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 16:17:07 by houabell          #+#    #+#             */
/*   Updated: 2025/06/13 10:08:20 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command	*parse_tokens(t_shell	*shell)
{
	if (shell && shell->tokens)
	{
		if (shell->variables)
		{
			expand_variables(shell);
		}
		if (check_ambig(shell) != SUCCESS)
			return (NULL);
		if (handle_heredocs(shell) != SUCCESS)
			return (NULL);
		create_commands(shell);
		return (shell->commands);
	}
	return (NULL);
}
