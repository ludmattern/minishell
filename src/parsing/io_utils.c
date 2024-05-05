/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:23:43 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/05 21:34:12 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

void	set_io_type(t_io_type *type, char **c)
{
	if (**c == '>')
	{
		*type = IO_OUT;
		(*c)++;
		if (**c == '>')
		{
			*type = IO_APPEND;
			(*c)++;
		}
	}
	else if (**c == '<')
	{
		*type = IO_IN;
		(*c)++;
		if (**c == '<')
		{
			*type = IO_HEREDOC;
			(*c)++;
		}
	}
}

int	countq(const char *str)
{
	int	count;

	count = 0;
	while (*str && *str != '>' && *str != '<' && *str != ' ')
	{
		if (*str == '"')
			count++;
		str++;
	}
	return (count);
}

void	toggle_quote(int *iq, char **c)
{
	if (**c == '"')
	{
		*iq = !*iq;
		(*c)++;
	}
}
