/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntaxe_checker.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:53:14 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/12 14:22:20 by fprevot          ###   ########.fr       */
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

bool check_par(const char *cmd)
{
    int i = 0;
    int par_count = 0;
    char current_quote = 0;

    while (cmd[i])
    {
        if ((cmd[i] == '\'' || cmd[i] == '\"') && (i == 0 || cmd[i - 1] != '\\'))
		{
            if (current_quote == 0)
                current_quote = cmd[i];
            else if (current_quote == cmd[i])
                current_quote = 0;
        }
        if (current_quote == 0)
		{
            if (cmd[i] == '(')
                par_count++;
            else if (cmd[i] == ')')
                par_count--;
            if (par_count < 0) 
                return (false);
        }
        i++;
    }
    return (par_count == 0);
}

bool check_sep(const char *cmd) 
{
    int i = 0;
    bool squotes = false, dquotes = false;

    while (cmd[i] == ' ')
        i++;
    if (cmd[i] == '|' || cmd[i] == '&')
        return (false);
    while (cmd[i])
    {
        if (cmd[i] == '\'' && !dquotes) 
            squotes = !squotes;
        else if (cmd[i] == '"' && !squotes)  
            dquotes = !dquotes;
        if (!squotes && !dquotes)
        {
            if ((cmd[i] == '|' || cmd[i] == '&') || (cmd[i] == '>' || cmd[i] == '<'))
            {
                if (cmd[i] == cmd[i + 1]) 
                    i++;
                i++;
                while (cmd[i] == ' ')
                    i++;
                if ((cmd[i] == '\0' || (cmd[i] == '|' || cmd[i] == '&') || (cmd[i] == '>' || cmd[i] == '<'))) 
                    return (false);
            }
        }
        i++;
    }
    return (true);
}

bool	syntax_error(const char *cmd, int *status)
{
	if (!check_dquotes(cmd))
	{
		ft_eprintf(MS"Parse Error: bad quotes\n");
		*status = EXIT_SYNTAX_ERROR;
		return (true);
	}
	if (!check_squotes(cmd))
	{
		ft_eprintf(MS"Parse Error: bad quotes\n");
		*status = EXIT_SYNTAX_ERROR;
		return (true);
	}
	if (!check_par(cmd))
	{
		ft_eprintf(MS"Parse Error: bad parentheses\n");
		*status = EXIT_SYNTAX_ERROR;
		return (true);
	}
    if (!check_sep(cmd))
	{
		ft_eprintf(MS"syntax error near unexpected sep\n");
		*status = EXIT_SYNTAX_ERROR;
		return (true);
	}
	return (false);
}
