/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 00:15:30 by houabell          #+#    #+#             */
/*   Updated: 2025/06/21 14:58:07 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	append_segment(char **buffer, char *part)
{
	char	*joined;

	if (!buffer || !part)
		return ;
	if (!*buffer)
		*buffer = ft_strdup(part);
	else
	{
		joined = ft_strjoin(*buffer, part);
		if (joined)
		{
			free(*buffer);
			*buffer = joined;
		}
	}
}

char	*get_env_value(char *name, t_shell *shell)
{
	t_env	*env;

	if (!name || !shell)
		return (ft_strdup(""));
	if (ft_strcmp(name, "?") == 0)
		return (ft_itoa(shell->exit_status));
	env = shell->env;
	while (env)
	{
		if (ft_strcmp(env->key, name) == 0)
		{
			if (env->value)
				return (ft_strdup(env->value));
			else
				return (NULL);
		}
		env = env->next;
	}
	return (NULL);
}

char	**ensure_valid_result(char **result)
{
	if (!result)
	{
		result = malloc(sizeof(char *) * 2);
		if (result)
		{
			result[0] = ft_strdup("");
			result[1] = NULL;
		}
	}
	return (result);
}
