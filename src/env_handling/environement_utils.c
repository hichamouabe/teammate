/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 22:03:48 by houabell          #+#    #+#             */
/*   Updated: 2025/06/25 16:01:50 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_my_env(char *name, t_env *env)
{
	t_env	*tmp;

	if (!name || !env)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, name) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

void	free_env(t_env *env)
{
	t_env	*temp;

	while (env)
	{
		temp = env;
		env = env->next;
		free(temp->key);
		if (temp->value)
			free(temp->value);
		free(temp);
	}
}

t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (!node->key)
		return (free(node), NULL);
	if (value)
	{
		node->value = ft_strdup(value);
		if (!node->value)
			return (free(node->key), free(node), NULL);
	}
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}

void	add_env_node(t_env **env, t_env *new)
{
	t_env	*temp;

	if (!*env)
	{
		*env = new;
		return ;
	}
	temp = *env;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

t_env	*get_env_node(t_env *env, char *key)
{
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}
