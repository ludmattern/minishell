/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntaxe_checker.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:53:14 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/14 14:59:23 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

bool	check_quotes(const char *cmd)
{
	int	quote;
	int	i;

	i = 0;
	quote = 0;
	while (cmd[i])
	{
		if (cmd[i] == '"')
			quote++;
		i++;
	}
	return (quote % 2 == 0);
}

bool	check_par(const char *cmd)
{
	int	i;
	int	par_count;

	i = 0;
	par_count = 0;
	while (cmd[i])
	{
		if (cmd[i] == '(')
			par_count++;
		if (cmd[i] == ')')
			par_count--;
		if (par_count < 0)
			return (false);
		i++;
	}
	return (par_count == 0);
}

bool	check_sep(const char *cmd)
{
	const char	*sep;
	int			i;

	sep = "|&<>";
	i = 0;
	while (cmd[i])
	{
		if (ft_strchr(sep, cmd[i]) != NULL)
		{
			if (i == 0 || cmd[i + 1] == '\0')
				return (false);
			if (cmd[i] == cmd[i + 1])
				return (false);
		}
		i++;
	}
	return (true);
}

bool	check_syntax(const char *cmd)
{
	if (!check_quotes(cmd))
	{
		printf("Parse Error: bad quotes\n");
		return (false);
	}
	if (!check_par(cmd))
	{
		printf("Parse Error: bad parentheses\n");
		return (false);
	}
	if (!check_sep(cmd))
	{
		printf("Parse Error: bad use of special characters\n");
		return (false);
	}
	return (true);
}
