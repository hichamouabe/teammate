/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 10:55:05 by houabell          #+#    #+#             */
/*   Updated: 2025/06/13 10:27:33 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*handle_variable_expansion(char *line, int *i, t_shell *shell)
{
	char	*var_name;
	char	*var_value;
	int		name_len;

	(*i)++;
	var_name = extract_var_name(&line[*i], &name_len);
	if (var_name)
	{
		var_value = get_env_value(var_name, shell);
		free(var_name);
		*i += name_len;
		return (var_value);
	}
	else if (line[*i] == '?')
	{
		var_value = ft_itoa(shell->exit_status);
		(*i)++;
		return (var_value);
	}
	return (ft_strdup("$"));
}

static char	*append_literal_part(char *final_line, \
char *line, int start, int end)
{
	char	*part;

	if (end > start)
	{
		part = ft_strndup(&line[start], end - start);
		if (part)
		{
			final_line = append_str(final_line, part);
			free(part);
		}
	}
	return (final_line);
}

static char	*process_dollar_sign(char *final_line, char *line, int *i, \
		t_shell *shell)
{
	char	*var_value;

	var_value = handle_variable_expansion(line, i, shell);
	final_line = append_str(final_line, var_value);
	free(var_value);
	return (final_line);
}

char	*expand_heredoc_line(char *line, t_shell *shell)
{
	char	*final_line;
	int		i;
	int		start;

	final_line = ft_strdup("");
	i = 0;
	start = 0;
	while (line[i])
	{
		if (line[i] == '$' && is_allowed(line[i + 1]))
		{
			final_line = append_literal_part(final_line, line, start, i);
			final_line = process_dollar_sign(final_line, line, &i, shell);
			start = i;
		}
		else
			i++;
	}
	final_line = append_literal_part(final_line, line, start, i);
	return (final_line);
}
