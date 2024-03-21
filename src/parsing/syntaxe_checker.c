/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntaxe_checker.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:53:14 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/21 13:49:58 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

bool check_squotes(const char *cmd) 
{
    bool squote = false;
    bool dquote = false;
    int i = 0;

    while (cmd[i])
	{
        if (cmd[i] == '"' && !squote)
            dquote = !dquote; 
        else if (cmd[i] == '\'' && !dquote)
            squote = !squote; 
        i++;
    }
    return (!squote); 
}

bool check_dquotes(const char *cmd)
{
    bool squote = false;
    bool dquote = false;
    int i = 0;

    while (cmd[i]) {
        if (cmd[i] == '\'' && !dquote)
            squote = !squote;
        else if (cmd[i] == '"' && !squote)
            dquote = !dquote;
        i++;
    }
    return (!dquote);
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
	if (!check_dquotes(cmd))
	{
		printf("Parse Error: bad quotes\n");
		return (false);
	}
	if (!check_squotes(cmd))
	{
		printf("Parse Error: bad quotes\n");
		return (false);
	}
	if (!check_par(cmd))
	{
		printf("Parse Error: bad parentheses\n");
		return (false);
	}

	return (true);
}
