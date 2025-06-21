/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_heredoc_filename.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 10:28:31 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/13 10:28:35 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*generate_heredoc_filename(t_shell *shell)
{
	char	*pid_str;
	char	*count_str;
	char	*base_name;
	char	*full_path;

	pid_str = ft_itoa(getpid());
	count_str = ft_itoa(shell->heredoc_count++);
	base_name = ft_strjoin("minishell_heredoc_", pid_str);
	base_name = append_str(base_name, "_");
	base_name = append_str(base_name, count_str);
	full_path = ft_strjoin("/tmp/", base_name);
	free(pid_str);
	free(count_str);
	free(base_name);
	add_segment(&shell->heredoc_files, ft_strdup(full_path));
	return (full_path);
}
