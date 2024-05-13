/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntaxe_first_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:39:19 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/13 10:23:43 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/bonus/parse_bonus.h"
#include "../../inc/bonus/exec_bonus.h"

void	toggle_quotes(bool *in_quotes, const char *cmd, int i)
{
	if (cmd[i] == '\'' || cmd[i] == '"')
	{
		if (!*in_quotes || (*in_quotes && cmd[i - 1] != '\\'))
			*in_quotes = !*in_quotes;
	}
}

int	skips(const char *cmd, int i)
{
	while (cmd[i] == ' ')
		i++;
	return (i);
}

bool	check_special_chars(const char *cmd, int i)
{
	if (cmd[i] == '|' || cmd[i] == '&')
		return (false);
	return (true);
}

bool	check_first(const char *cmd)
{
	bool	in_quotes;
	int		i;

	in_quotes = false;
	i = 0;
	while (cmd[i] != '\0')
	{
		toggle_quotes(&in_quotes, cmd, i);
		if (!in_quotes)
		{
			i = skips(cmd, i);
			if (!check_special_chars(cmd, i))
				return (false);
			else
				return (true);
		}
		i++;
	}
	return (true);
}
