/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:55:05 by houabell          #+#    #+#             */
/*   Updated: 2025/06/21 08:32:58 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	create_heredoc_file(t_shell *shell)
{
	char	*temp_filename;
	int		fd;

	temp_filename = generate_heredoc_filename(shell);
	if (!temp_filename)
		return (-1);
	fd = open(temp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	return (free(temp_filename), fd);
}

static int	should_stop_heredoc(char *line, char *delimiter)
{
	if (!line || ft_strcmp(line, delimiter) == 0)
		return (1);
	return (0);
}

static char	*process_heredoc_line(char *line, int expand, t_shell *shell)
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

static void	write_line_to_file(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

int	read_heredoc_input(char *delimiter, int expand, t_shell *shell)
{
	char	*line;
	int		fd;

	fd = create_heredoc_file(shell);
	if (fd == -1)
		return (ERROR);
	g_signal_status = 0;
	signal(SIGINT, sigint_heredoc_handler);
	while (1)
	{
		line = readline("> ");
		if (g_signal_status == 1)
			break ;
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
	signal(SIGINT, sigint_handler);
	return (close(fd), SUCCESS);
}
