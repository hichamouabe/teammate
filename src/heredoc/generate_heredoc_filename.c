/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_heredoc_filename.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 10:28:31 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/25 16:14:22 by aelbouz          ###   ########.fr       */
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

int	process_heredoc_loop(char *delimiter, int expand, int fd, t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (g_signal_status == 130)
		{
			shell->heredoc_sigint = 1;
			break ;
		}
		if (should_stop_heredoc(line, delimiter))
		{
			if (line)
				free(line);
			break ;
		}
		line = process_heredoc_line(line, expand, shell);
		write_line_to_file(fd, line);
		free(line);
	}
	return (0);
}
