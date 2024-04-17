/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntaxe_checker.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:53:14 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/17 13:03:17 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

bool	check_squotes(const char *cmd)
{
	bool	squote;
	bool	dquote;
	int		i;

	squote = false;
	dquote = false;
	i = 0;
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

bool	check_dquotes(const char *cmd)
{
	bool	squote;
	bool	dquote;
	int		i;

	squote = false;
	dquote = false;
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\'' && !dquote)
			squote = !squote;
		else if (cmd[i] == '"' && !squote)
			dquote = !dquote;
		i++;
	}
	return (!dquote);
}

bool	check_par(const char *cmd, char **token)
{
	int i = 0;
	int par_count = 0;
	char current_quote = 0;
	bool empty_par = false;

	i = 0;
	par_count = 0;
	current_quote = 0;
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
			{
				par_count++;
				empty_par = true;
			}
			else if (cmd[i] == ')')
			{
				par_count--;
				if (empty_par)
					break;
			}
			else if (!ft_isspace(cmd[i]))
				empty_par = false;
			if (par_count < 0)
				break;
		}
		i++;
	}
	if (empty_par || par_count < 0)
		return (*token = ft_strdup(")"), false);
	else if (par_count > 0)
		return (*token = ft_strdup("("), false);
	return (true);
}

bool check_redir(const char *cmd, char **token)
{
	bool in_quotes = false;
	int i = 0;
	int j = 0;
	while (cmd[i] != '\0')
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
		{
			if (!in_quotes || (in_quotes && cmd[i - 1] != '\\'))
				in_quotes = !in_quotes;
		}
		if (!in_quotes)
		{
			if (cmd[i] == '>' || cmd[i] == '<')
			{
				if (cmd[i + 1] == '>' || cmd[i + 1] == '<')
					i++;
				if (cmd[i + 1] == '>' || cmd[i + 1] == '<')
					i++;
				j = i + 1;
				while (cmd[j] == ' ')
					j++;
				if (cmd[j] == '\0' || cmd[j] == '|' || cmd[j] == '&' || cmd[j] == '<' || cmd[j] == '>' || cmd[j] == '<' || cmd[j] == '>')
				{
					if (cmd[j] == '\0')
						*token = ft_strdup("newline");
					else
					{
						*token = malloc(sizeof(char) * 3);
						if (!*token)
						{
							perror("malloc");
							return (false);
						}
						(*token)[0] = (char)cmd[j];
						if (cmd[j + 1] == (char)cmd[j])
						{
							(*token)[1] = (char)cmd[j];
							(*token)[2] = '\0';
						}
						else
						{
							(*token)[1] = '\0';
							(*token)[2] = '\0';
						}
					}
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
					if (cmd[j] == '\0')
						*token = ft_strdup("newline");
					else
					{
						*token = malloc(sizeof(char) * 3);
						if (!*token)
						{
							perror("malloc");
							return (false);
						}
						(*token)[0] = (char)cmd[j];
						if (cmd[j + 1] == (char)cmd[j])
						{
							(*token)[1] = (char)cmd[j];
							(*token)[2] = '\0';
						}
						else
						{
							(*token)[1] = '\0';
							(*token)[2] = '\0';
						}
					}
					return (false);
				}
			}
		}
		i++;
	}
	return (true);
}

bool check_first(const char *cmd, char **token)
{
	bool in_quotes = false;
	int i = 0;

	while (cmd[i] != '\0')
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
		{
			if (!in_quotes || (in_quotes && cmd[i - 1] != '\\'))
				in_quotes = !in_quotes;
		}
		if (!in_quotes)
		{
			while (cmd[i] == ' ')
				i++;
			if (cmd[i] == '|' || cmd[i] == '&')
			{
				*token = malloc(sizeof(char) * 3);
				if (!*token)
				{
					perror("malloc");
					return (false);
				}
				(*token)[0] = (char)cmd[i];
				if (cmd[i + 1] == (char)cmd[i])
				{
					(*token)[1] = (char)cmd[i];
					(*token)[2] = '\0';
				}
				else
				{
					(*token)[1] = '\0';
					(*token)[2] = '\0';
				}
				return (false);
			}
			else
				return (true);
		}
		i++;
	}
	return (true);
}
