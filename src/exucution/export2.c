/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:31:38 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/18 11:35:08 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	update_existing_key(t_env *tmp, char *key, char *value)
{
	if (tmp->value && !value)
		return (1);
	free(tmp->value);
	free(tmp->key);
	tmp->key = ft_strdup(key);
	if (!tmp->key)
		return (0);
	if (value)
		tmp->value = ft_strdup(value);
	else
		tmp->value = NULL;
	return (1);
}

int	find_and_update(t_env **env, char *key, char *value)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (update_existing_key(tmp, key, value));
		tmp = tmp->next;
	}
	return (0);
}

t_env	*new_copie(t_env *env)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(env->key);
	if (!new->key)
		return (free(new), NULL);
	if (env->value)
		new->value = ft_strdup(env->value);
	else
		new->value = NULL;
	if (env->value && !new->value)
		return (free(new->key), free(new), NULL);
	new->next = NULL;
	return (new);
}

t_env	*copie_env(t_env	*env)
{
	t_env	*copie;
	t_env	*new;
	t_env	*last;

	last = NULL;
	copie = NULL;
	while (env)
	{
		new = new_copie(env);
		if (!copie)
			copie = new;
		else
			last->next = new;
		last = new;
		env = env->next;
	}
	return (copie);
}

t_env	*env_sorted(t_env *env)
{
	t_env	*current;
	char	*tmp_key;
	char	*tmp_value;

	current = env;
	while (current && current->next)
	{
		if (strcmp(current->key, current->next->key) > 0)
		{
			tmp_key = current->key;
			current->key = current->next->key;
			current->next->key = tmp_key;
			tmp_value = current->value;
			current->value = current->next->value;
			current->next->value = tmp_value;
			current = env;
		}
		else
			current = current->next;
	}
	return (env);
}
