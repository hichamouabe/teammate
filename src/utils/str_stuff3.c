/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_stuff3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 01:30:51 by houabell          #+#    #+#             */
/*   Updated: 2025/06/02 01:41:05 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	if (s1 == NULL)
		return (ft_strdup(s2));
	if (s2 == NULL)
		return (ft_strdup(s1));
	result = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (result == NULL)
		return (NULL);
	ft_strlcpy(result, s1, ft_strlen(s1) + 1);
	ft_strlcat(result, s2, ft_strlen(s1) + ft_strlen(s2) + 1);
	return (result);
}

int	ft_isalpha(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	srclen;
	size_t	dstlen;

	srclen = ft_strlen(src);
	if (dstsize == 0)
		return (srclen);
	dstlen = ft_strlen(dst);
	if (dstsize <= dstlen)
		return (dstsize + srclen);
	ft_strlcpy(dst + dstlen, src, dstsize - dstlen);
	return (srclen + dstlen);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t destsize)
{
	size_t	copy_len;
	size_t	src_len;

	src_len = ft_strlen(src);
	if (destsize > 0)
	{
		if (destsize - 1 > src_len)
			copy_len = src_len;
		else
			copy_len = destsize - 1;
		ft_memcpy(dst, src, copy_len);
		dst[copy_len] = '\0';
	}
	return (src_len);
}
