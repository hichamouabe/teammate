/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_stuff1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 01:23:30 by houabell          #+#    #+#             */
/*   Updated: 2025/06/05 20:35:12 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

size_t	ft_strlen(const char *s)
{
	size_t	len;

	if (s == NULL)
		return (0);
	len = 0;
	while (s[len])
	{
		len++;
	}
	return (len);
}

char	*ft_strdup(const char *s1)
{
	size_t	len;
	size_t	i;
	char	*ptr;

	if (s1 == NULL)
		return (NULL);
	len = ft_strlen(s1);
	ptr = malloc(len + 1);
	i = 0;
	if (ptr == NULL)
		return (NULL);
	while (i < len)
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char	*ft_strndup(const char	*s1, size_t n)
{
	size_t	i;
	size_t	len;
	char	*ptr;

	len = ft_strlen(s1);
	if (len > n)
		len = n;
	ptr = malloc(len + 1);
	i = 0;
	while (i < len)
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char	*ft_strchr(const char *s, int c)
{
	char	*p;

	p = (char *)s;
	c = (char)c;
	while (*p)
	{
		if (*p == c)
		{
			return (p);
		}
		p++;
	}
	if (*p == c)
	{
		return (p);
	}
	return (0);
}
