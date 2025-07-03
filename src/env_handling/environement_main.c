/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement_main.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 21:51:40 by houabell          #+#    #+#             */
/*   Updated: 2025/07/02 14:12:49 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_env	*create_env_node_from_string(char *env_string)
{
	char	*key;
	char	*value;
	t_env	*node;

	key = ft_strdup(env_string);
	if (!key)
		return (NULL);
	value = ft_strchr(key, '=');
	if (value)
	{
		*value = '\0';
		value++;
		node = new_env_node(key, value);
		free(key);
		if (!node)
			return (NULL);
		return (node);
	}
	free(key);
	return (NULL);
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*node;
	int		i;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		node = create_env_node_from_string(envp[i]);
		if (!node)
		{
			free_env(env);
			return (NULL);
		}
		add_env_node(&env, node);
		i++;
	}
	return (env);
}

t_env	*ft_lstnew(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->next = NULL;
	return (new);
}

void	init_default_env1(t_env **env)
{
	t_env	*new;
	char	*pwd;

	if (!get_my_env("PWD", *env))
	{
		pwd = getcwd(NULL, 0);
		if (pwd)
		{
			new = ft_lstnew("PWD", pwd);
			ft_lstadd_back(env, new);
			free(pwd);
		}
	}
}

t_env	*init_default_env(t_env **env)
{
	t_env	*new;
	char	*pwd;

	if (!get_my_env("PWD", *env))
	{
		pwd = getcwd(NULL, 0);
		if (pwd)
		{
			new = ft_lstnew("PWD", pwd);
			ft_lstadd_back(env, new);
			free(pwd);
		}
	}
	if (!get_my_env("PATH", *env))
	{
		new = ft_lstnew("PATH", \
		"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
		ft_lstadd_back(env, new);
	}
	if (!get_my_env("OLLPWD", *env))
	{
		new = ft_lstnew("OLDPWD", NULL);
		ft_lstadd_back(env, new);
	}
	return (*env);
}
