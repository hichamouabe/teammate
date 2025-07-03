/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 09:27:38 by aelbouz           #+#    #+#             */
/*   Updated: 2025/07/02 10:51:53 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

int	is_valide_args(char *key)
{
	int	i;

	if (!*key || !key || ft_isdigit(key[0]) || key[0] == '=' || key[0] == '+')
		return (0);
	i = 1;
	while (key[i] && key[i] != '=')
	{
		if (!ft_isalnum(key[i]) && key[i] != '_' && (key[i] == '+' && key[i + 1] != '='))
			return (0);
		i++;
	}
	return (1);
}

void	print_export(t_env *env)
{
	t_env	*sorted;
	t_env	*copie;

	copie = copie_env(env);
	sorted = env_sorted(copie);
	if (!sorted)
		return ;
	while (sorted)
	{
		if (!sorted->value && ft_strcmp(sorted->key, "_=") != 0)
			printf("declare -x %s\n", sorted->key);
		else if (ft_strcmp(sorted->key, "_=") != 0)
			printf("declare -x %s=\"%s\"\n", sorted->key, sorted->value);
		sorted = sorted->next;
	}
	free_env(sorted);
	free_env(copie);
}

int	handl_export_args(char *arg, t_env **env)
{
	char	*key;
	char	*value;
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		key = ft_substr(arg, 0, ft_strlen(arg) - ft_strlen(eq));
		if (!key)
			return (1);
		value = ft_substr(eq + 1, 0, ft_strlen(eq));
		if (!value)
			return (free(key), 1);
		updat_env(env, key, value);
		free(key);
		free(value);
	}
	else
		updat_env(env, arg, NULL);
	return (0);
}

int	ft_export(char **args, t_env **env)
{
	int		i;
	int		status;

	status = 0;
	if (!args[1])
		return (print_export(*env), status);
	i = 1;
	while (args[i])
	{
		if (!is_valide_args(args[i]))
		{
			ft_putstr_fd("minishell: export: '", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else if (handl_export_args(args[i], env))
			status = 1;
		i++;
	}
	return (status);
}
