/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:00:59 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/25 16:01:41 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	env_to_array2(char **envp, int	*i, t_env *tmp)
{
	char	*tmp_key;

	while (tmp)
	{
		if (!tmp->key && !tmp->value)
			return (free_arr(envp), 1);
		tmp_key = ft_strjoin(tmp->key, "=");
		envp[*i] = ft_strjoin(tmp_key, tmp->value);
		if (!envp[*i])
			return (free_arr(envp), free(tmp_key), 1);
		free(tmp_key);
		(*i)++;
		tmp = tmp->next;
	}
	return (0);
}
