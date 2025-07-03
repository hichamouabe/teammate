/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:55:05 by houabell          #+#    #+#             */
/*   Updated: 2025/06/25 16:22:14 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	create_heredoc_file(t_shell *shell)
{
	char	*temp_filename;
	int		fd;

	temp_filename = generate_heredoc_filename(shell);
	if (!temp_filename)
		return (-1);
	fd = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	return (free(temp_filename), fd);
}

int	should_stop_heredoc(char *line, char *delimiter)
{
	if (!line || ft_strcmp(line, delimiter) == 0)
		return (1);
	return (0);
}

char	*process_heredoc_line(char *line, int expand, t_shell *shell)
{
	char	*expanded;

	if (expand)
	{
		expanded = expand_heredoc_line(line, shell);
		free(line);
		return (expanded);
	}
	return (line);
}

void	write_line_to_file(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

int	read_heredoc_input(char *delimiter, int expand, t_shell *shell)
{
	int		fd;
	int		stdin_backup;

	fd = create_heredoc_file(shell);
	if (fd == -1)
		return (ERROR);
	g_signal_status = 0;
	stdin_backup = dup(STDIN_FILENO);
	signal(SIGINT, sigint_heredoc_handler);
	process_heredoc_loop(delimiter, expand, fd, shell);
	signal(SIGINT, sigint_handler);
	close(fd);
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
	if (shell->heredoc_sigint)
		return (ERROR);
	return (SUCCESS);
}
