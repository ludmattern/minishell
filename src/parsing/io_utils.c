/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:23:43 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/03 13:10:35 by lmattern         ###   ########.fr       */
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

char	*extract_with_quote(char **c, t_g_data *data)
{
	char	*output;
	int		iq;
	char	*result;

	result = malloc(ft_strlen(*c) + 1);
	if (!result)
		fail_exit_shell(data);
	output = result;
	iq = 0;
	while (**c && **c != '>' && **c != '<')
	{
		toggle_quote(&iq, c);
		if (**c && !iq && **c != '"' && **c != '>' && **c != '<')
		{
			*output++ = **c;
			(*c)++;
		}
		else if (iq)
		{
			*output++ = **c;
			(*c)++;
		}
	}
	*output = '\0';
	return (result);
}
