/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_var_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:21:53 by houabell          #+#    #+#             */
/*   Updated: 2025/06/05 16:28:10 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*extract_var_name(char *str_after_dol, int *name_len)
{
	int		i;
	char	*name;

	*name_len = 0;
	i = 0;
	if (!str_after_dol || !(*str_after_dol))
		return (NULL);
	if (!(ft_isalpha(str_after_dol[0]) || str_after_dol[0] == '_'))
		return (NULL);
	while (str_after_dol[i] && \
			(ft_isalnum(str_after_dol[i]) || str_after_dol[i] == '_') && \
			str_after_dol[i] != '$')
		i++;
	if (i == 0)
		return (NULL);
	*name_len = i;
	name = ft_strndup(str_after_dol, i);
	return (name);
}

void	handle_question_mark(t_shell *shell, int *index)
{
	t_var_info	*var_node;

	var_node = create_var_node("?", shell->csqt);
	add_var_node(&(shell->variables), var_node);
	*index += 2;
}

void	process_variable(t_shell *shell, char *segment, int *index)
{
	int			name_length;
	char		*extracted_name;
	t_var_info	*var_node;

	if (segment[*index + 1] == '\0')
	{
		(*index)++;
		return ;
	}
	if (segment[*index + 1] == '?')
	{
		handle_question_mark(shell, index);
		return ;
	}
	extracted_name = extract_var_name(&segment[*index + 1], &name_length);
	if (extracted_name)
	{
		var_node = create_var_node(extracted_name, shell->csqt);
		add_var_node(&(shell->variables), var_node);
		free(extracted_name);
		*index += (1 + name_length);
	}
	else
		(*index)++;
}

void	log_expandable(t_shell *shell, char *segment)
{
	int	i;

	i = 0;
	if (!shell || !segment)
		return ;
	while (segment[i])
	{
		if (segment[i] == '$')
			process_variable(shell, segment, &i);
		else
			i++;
	}
}
