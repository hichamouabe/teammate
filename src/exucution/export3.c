/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 09:18:01 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/19 10:58:11 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	creat_new_node(t_env **env, char *key, char *value)
{
	t_env	*tmp;

	tmp = malloc(sizeof(t_env));
	if (!tmp)
		return (free(key));
	tmp->key = ft_strdup(key);
	if (value)
		tmp->value = ft_strdup(value);
	else
		tmp->value = NULL;
	if (value && !tmp->value)
		return (free(tmp->key), free(tmp), free(key));
	tmp->next = NULL;
	ft_lstadd_back(env, tmp);
}

void	handl_plus(t_env **env, char *key, char *value)
{
	char	*tmp_key;
	char	*tmp_value;
	t_env	*tmp;

	tmp_key = ft_strtrim(key, "+=");
	if (!tmp_key)
		return ;
	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, tmp_key) == 0)
		{
			tmp_value = tmp->value;
			tmp->value = ft_strjoin(tmp_value, value);
			return (free(tmp_value), free(tmp_key));
		}
		tmp = tmp->next;
	}
	creat_new_node(env, tmp_key, value);
	free(tmp_key);
}

void	updat_env(t_env **env, char *key, char *value)
{
	t_env	*tmp;

	if (ft_strchr(key, '+'))
		return (handl_plus(env, key, value));
	if (find_and_update(env, key, value))
		return ;
	tmp = malloc(sizeof(t_env));
	if (!tmp)
		return (free(key));
	tmp->key = ft_strdup(key);
	if (value)
		tmp->value = ft_strdup(value);
	else
		tmp->value = NULL;
	if (value && !tmp->value)
		return (free(tmp->key), free(tmp));
	tmp->next = NULL;
	ft_lstadd_back(env, tmp);
}
