/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_utils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:32:22 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/13 10:24:50 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/bonus/parse_bonus.h"

void	imoremore_quote(char *arg, int *i, char c)
{
	(*i)++;
	while (arg[*i] && arg[*i] != c)
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

bool	is_previous_heredoc(int i, char *res)
{
	int	j;

	if (i == 0)
		return (true);
	j = i - 1;
	while (res && j > 0 && res[j] != '>' && res[j] != '<')
		j--;
	if (res[j] == '<' && res[j - 1] == '<')
		return (false);
	else
		return (true);
}

bool	is_end(char *str, int i)
{
	while (str[i] != '\0')
	{
		if (!ft_isspace(str[i]))
			return (false);
		i++;
	}
	return (true);
}
