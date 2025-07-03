/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_stuff4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 01:36:31 by houabell          #+#    #+#             */
/*   Updated: 2025/06/13 09:53:58 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_allowed(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	if (c >= '0' && c <= '9')
		return (1);
	if (c == '_' || c == '?')
		return (1);
	return (0);
}

void	free_arr(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	*ft_strcpy(char *dst, const char *src)
{
	char	*origin_dst;

	origin_dst = dst;
	while (*src != '\0')
	{
		*dst = *src;
		dst++;
		src++;
	}
	*dst = '\0';
	return (origin_dst);
}

char	*ft_strcat(char *dst, const char *src)
{
	char	*origin_dst;

	origin_dst = dst;
	while (*dst != '\0')
		dst++;
	while (*src != '\0')
	{
		*dst = *src;
		dst++;
		src++;
	}
	*dst = '\0';
	return (origin_dst);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;

	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	if (dest == src)
		return (dest);
	if (dest == NULL && src == NULL)
		return (NULL);
	while (n--)
		*d++ = *s++;
	return (dest);
}
