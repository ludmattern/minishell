/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:32:22 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/18 16:47:59 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

void	imoremore_quote(char *arg, int *i, char c)
{
	(*i)++;
	while (arg[*i] != c)
		(*i)++;
}

void	imore(int size, int *i)
{
	int	k;

	k = -1;
	while (++k < size)
		(*i)++;
}

void	skip_space(char *input, int *index)
{
	while (input[*index] == ' ')
		(*index)++;
}
