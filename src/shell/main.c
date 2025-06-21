/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 22:07:58 by houabell          #+#    #+#             */
/*   Updated: 2025/06/05 17:40:55 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

volatile int	g_signal_status;

int	main(int ac, char **av, char **envp)
{
	t_shell	*shell;
	int		exit_status;

	(void)ac;
	(void)av;
	shell = init_shell(envp);
	if (!shell)
		return (ERROR);
	exit_status = minishell_loop(shell);
	free_shell(shell);
	return (exit_status);
}
