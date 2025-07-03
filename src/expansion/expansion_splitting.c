/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_splitting.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: houabell <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 22:08:47 by houabell          #+#    #+#             */
/*   Updated: 2025/06/05 16:44:29 by houabell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	process_word_splitting(t_expansion_state *state, char *expanded)
{
	char	**split;

	split = ft_split(expanded, ' ');
	if (split)
		distribute_split_words(state, split);
	else if (*expanded)
		state->buffer = ft_strdup(expanded);
}
