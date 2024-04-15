/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntaxe_checker.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:53:14 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/15 11:36:07 by fprevot          ###   ########.fr       */
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



bool check_redir(const char *cmd) 
{
    bool in_quotes = false;
    int i = 0;
 	int j = 0;
    while (cmd[i] != '\0') 
	{
        if (cmd[i] == '\'' || cmd[i] == '"') 
		{
            if (!in_quotes || (in_quotes && cmd[i-1] != '\\'))
                in_quotes = !in_quotes;
        }
        if (!in_quotes) 
		{
            if (cmd[i] == '>' || cmd[i] == '<') 
			{
                j = i + 1;
                while (cmd[j] == ' ') 
					j++;  
                if (cmd[j] == '\0' || cmd[j] == '|' || cmd[j] == '&') 
				{
                    return (false);  
                }
            }
            if (cmd[i] == '|' || cmd[i] == '&') 
			{
				if (cmd[i + 1] == '|' || cmd[i + 1] == '&') 
					i++;
                j = i + 1;
                while (cmd[j] == ' ') 
					j++;
                if (cmd[j] == '\0' || cmd[j] == '|' || cmd[j] == '&') 
				{
                    return (false);
                }
            }
        }
        i++;
    }
    return (true);
}

bool check_first(const char *cmd) 
{
    bool in_quotes = false;
    int i = 0;
	
 	
    while (cmd[i] != '\0') 
	{
        if (cmd[i] == '\'' || cmd[i] == '"') 
		{
            if (!in_quotes || (in_quotes && cmd[i-1] != '\\'))
                in_quotes = !in_quotes;
        }
        if (!in_quotes) 
		{
            while (cmd[i] == ' ') 
					i++;
			if(cmd[i] == '|' || cmd[i] == '&')
				return (false);
			else
				return (true);
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
	if (!check_redir(cmd))
	{
		ft_eprintf(MS"Parse Error: bad token\n");
		*status = EXIT_SYNTAX_ERROR;
		return (true);
	}
	if (!check_first(cmd))
	{
		ft_eprintf(MS"Parse Error: bad token\n");
		*status = EXIT_SYNTAX_ERROR;
		return (true);
	}
	return (false);
}
