/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_stuff2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 01:26:11 by houabell          #+#    #+#             */
/*   Updated: 2025/06/02 01:29:59 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while ((*s1 || *s2))
	{
		if (*s1 != *s2)
			return ((unsigned char)*s1 - (unsigned char)*s2);
		s1++;
		s2++;
	}
	return (0);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	s_len;
	size_t	i;

	i = 0;
	if (s == NULL)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	substr = malloc(len + 1);
	if (substr == NULL)
		return (NULL);
	while (i < len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

static int	ft_calclen(int n)
{
	size_t	tmp;
	int		len;

	tmp = n;
	len = 0;
	if (n < 0)
	{
		len = 1;
		tmp = -tmp;
	}
	while (tmp)
	{
		tmp /= 10;
		len++;
	}
	if (n == 0)
		return (1);
	return (len);
}

static char	*ft_converter(int n, int len)
{
	char	*result;
	long	num;

	num = n * 1;
	result = (char *)malloc(len + 1);
	if (!result)
		return (NULL);
	result[len] = '\0';
	if (num < 0)
	{
		result[0] = '-';
		num = -num;
	}
	while (len > 0)
	{
		len--;
		result[len] = (num % 10) + '0';
		num /= 10;
		if (num == 0 && result[0] == '-')
			break ;
	}
	return (result);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*result;

	len = ft_calclen(n);
	result = ft_converter(n, len);
	return (result);
}
