/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntaxe_par.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:29:16 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/08 20:16:37 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

bool	process_closed_par(const char *cmd, int i)
{
	if (cmd && cmd[i] == ')')
	{
		i++;
		while (ft_isspace(cmd[i]))
			i++;
		if (cmd[i] && (cmd[i] != '<'
				&& cmd[i] != '>'
				&& cmd[i] != '|'
				&& cmd[i] != '&'
				&& cmd[i] != ')'))
		{
			return (false);
		}
	}
	return (true);
}

void	update_quote_status(char c, char *current_quote, \
int i, const char *cmd)
{
	if ((c == '\'' || c == '\"') && (i == 0 || cmd[i - 1] != '\\'))
	{
		if (*current_quote == 0)
			*current_quote = c;
		else if (*current_quote == c)
			*current_quote = 0;
	}
}

bool	process_parentheses(char c, int *par_count, bool *empty_par)
{
	if (c == '(')
	{
		(*par_count)++;
		*empty_par = true;
	}
	else if (c == ')')
	{
		(*par_count)--;
		if (*empty_par)
		{
			return (false);
		}
	}
	else if (!ft_isspace(c))
		*empty_par = (false);
	return (true);
}

bool	check_par(const char *cmd, int i, int par_count)
{
	char	current_quote;
	bool	empty_par;

	current_quote = 0;
	empty_par = false;
	while (cmd[i])
	{
		update_quote_status(cmd[i], &current_quote, i, cmd);
		if (current_quote == 0)
		{
			if (!process_parentheses(cmd[i], &par_count, &empty_par))
				break ;
			if (!process_closed_par(cmd, i))
				return (false);
			if (par_count < 0)
				break ;
		}
		i++;
	}
	if (empty_par || par_count < 0)
		return (false);
	else if (par_count > 0)
		return (false);
	return (true);
}
