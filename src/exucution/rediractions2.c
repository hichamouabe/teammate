/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rediractions2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:36:00 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/19 10:59:09 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_out_redir(t_redirect *redir_info)
{
	int	fd;

	if (!redir_info || !redir_info->file)
		return (-1);
	if (redir_info->type == TOKEN_REDIR_OUT)
		fd = open(redir_info->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (redir_info->type == TOKEN_REDIR_APPEND)
		fd = open(redir_info->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		return (-1);
	if (fd == -1)
		return (perror("minishell : open"), -1);
	return (fd);
}

int	handle_in_redir(t_redirect *redir_info)
{
	int	fd;

	if (!redir_info || !redir_info->file)
		return (-1);
	fd = open(redir_info->file, O_RDONLY);
	if (fd == -1)
		return (perror("minishell : open"), -1);
	return (fd);
}

int	handle_redir(t_command *cmd)
{
	t_redirect	*redir;
	int			in_fd;
	int			out_fd;

	in_fd = -1;
	out_fd = -1;
	cmd->file = -1;
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == TOKEN_REDIR_OUT || redir->type == TOKEN_REDIR_APPEND)
			out_fd = handle_out_redir(redir);
		else if (redir->type == TOKEN_REDIR_IN)
			in_fd = handle_in_redir(redir);
		redir = redir->next;
	}
	if (out_fd != -1)
		cmd->file = out_fd;
	else if (in_fd != -1)
		cmd->file = in_fd;
	return (0);
}

void	free_redirects(t_redirect *redir)
{
	t_redirect	*temp;

	while (redir)
	{
		temp = redir;
		redir = redir->next;
		if (temp->file)
			free(temp->file);
		free(temp);
	}
}

void	free_cmd(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->redirects)
		free_redirects(cmd->redirects);
	free(cmd);
}
