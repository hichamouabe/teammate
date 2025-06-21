/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:11:30 by houabell          #+#    #+#             */
/*   Updated: 2025/06/20 17:14:26 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	cleanup_files(t_shell *shell)
{
	int	i;

	i = 0;
	if (shell->heredoc_files)
	{
		while (shell->heredoc_files[i])
		{
			unlink(shell->heredoc_files[i]);
			i++;
		}
		free_arr(shell->heredoc_files);
		shell->heredoc_files = NULL;
	}
	shell->heredoc_count = 0;
}
