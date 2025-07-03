/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 00:28:52 by houabell          #+#    #+#             */
/*   Updated: 2025/06/17 20:38:53 by aelbouz       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_segment(char ***result, char *segment)
{
	int		i;
	char	**new_arr;

	if (!result)
	{
		if (segment)
			free(segment);
	}
	if (!segment)
		return ;
	i = 0;
	if (*result)
		while ((*result)[i])
			i++;
	new_arr = malloc(sizeof(char *) * (i + 2));
	if (!new_arr)
		return ;
	copy_existing_segments(new_arr, *result, i);
	new_arr[i] = segment;
	new_arr[i + 1] = NULL;
	if (*result)
		free(*result);
	*result = new_arr;
}

void	copy_existing_segments(char **new_arr, char **old_arr, int count)
{
	int	j;

	j = 0;
	if (old_arr)
	{
		while (j < count)
		{
			new_arr[j] = old_arr[j];
			j++;
		}
	}
}
