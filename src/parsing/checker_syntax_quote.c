/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntaxe_checker.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 14:53:14 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/19 15:42:18 by fprevot          ###   ########.fr       */
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
