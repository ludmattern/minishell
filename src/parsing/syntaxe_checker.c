/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntaxe_checker.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:53:14 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/17 13:57:28 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

bool	check_quotes(const char *cmd)
{
	int	squote;
	int dquote;
	int	i;

	i = 0;
	dquote = 0;
	squote = 0;
	while (cmd[i])
	{
		if (cmd[i] == '"')
			dquote++;
		else if (cmd[i] == '\'')
			squote++;
		i++;
	}
	if (dquote % 2 != 0)
		return (false);
	else if (squote % 2 != 0)
		return (false);
	else
		return (true);
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

bool check_sep(const char *cmd) 
{
	int	i;
	int j;
	
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '|' || cmd[i] == '&' || cmd[i] == '>' || cmd[i] == '<')
		{
			if (cmd[i + 1] == cmd[i])
				i++;
			i++;
			while (cmd[i] == ' ')
				i++;
			if (cmd[i] != '\0' && cmd[i] != ' ' && (cmd[i] == '|' \
				|| cmd[i] == '&' || cmd[i] == '<' || cmd[i] == '>')) 
                return (false);
			j = i + 1;
            while (cmd[j] == ' ')
				j++; 
            if (cmd[j] == '\0')
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
