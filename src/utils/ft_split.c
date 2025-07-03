/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 01:33:35 by houabell          #+#    #+#             */
/*   Updated: 2025/06/05 20:44:09 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	ft_error_handler(char **result, size_t k);

static size_t	ft_count(const char *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && s[i] != '\t' && \
				(i == 0 || s[i - 1] == c || s[i - 1] == '\t'))
			count++;
		i++;
	}
	return (count);
}

static char	**ft_devider(char **result, const char *s, char c, size_t count)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	k = 0;
	while (s[i] && k < count)
	{
		if (s[i] != c && s[i] != '\t')
		{
			j = i;
			while (s[j] && s[j] != c && s[j] != '\t')
				j++;
			result[k] = ft_substr(s, i, j - i);
			if (!result[k++])
			{
				ft_error_handler(result, k - 1);
				return (NULL);
			}
			i = j;
		}
		else
			i++;
	}
	return (result);
}

static void	ft_error_handler(char **result, size_t k)
{
	while (k > 0)
		free(result[--k]);
	free(result);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	size_t	count;

	if (!s)
		return (NULL);
	count = ft_count(s, c);
	result = (char **)malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	result[count] = NULL;
	if (!ft_devider(result, s, c, count))
		return (NULL);
	return (result);
}
