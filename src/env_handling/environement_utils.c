/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 22:03:48 by houabell          #+#    #+#             */
/*   Updated: 2025/06/19 13:15:35 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
