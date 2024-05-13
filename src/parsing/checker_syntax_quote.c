/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_syntax_quote.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:53:14 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/13 10:29:17 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/mandatory/parse.h"

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
