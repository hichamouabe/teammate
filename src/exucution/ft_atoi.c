/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:34:27 by aelbouz           #+#    #+#             */
/*   Updated: 2025/06/13 09:52:41 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static const char	*skip_whitespace(const char *str, int *sign)
{
	while (*str == ' ' || *str == '\n'
		|| *str == '\t' || *str == '\r'
		|| *str == '\v' || *str == '\f')
	{
		str++;
	}
	*sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
		{
			*sign = -1;
		}
		str++;
	}
	return (str);
}

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	sign = 1;
	str = skip_whitespace(str, &sign);
	result = 0;
	while (*str >= '0' && *str <= '9')
	{
		if (result > INT_MAX / 10)
		{
			if (sign == 1)
				return (0);
			else
				return (-1);
		}
		if (result == INT_MAX / 10)
		{
			if (sign == 1 && (*str - '0') > 7)
				return (0);
			else if (sign == -1 && (*str - '0') > 8)
				return (-1);
		}
		result = result * 10 + (*str++ - '0');
	}
	return (result * sign);
}
