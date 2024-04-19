/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntaxe_first.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:39:19 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/19 15:40:40 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"
#include "../../inc/exec.h"

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

bool	check_special_chars(const char *cmd, int i, char **token)
{
	if (cmd[i] == '|' || cmd[i] == '&')
	{
		*token = malloc(sizeof(char) * 3);
		if (!*token)
		{
			perror("malloc");
			return (false);
		}
		(*token)[0] = cmd[i];
		if (cmd[i + 1] == cmd[i])
			(*token)[1] = cmd[i];
		else
			(*token)[1] = '\0';
		(*token)[2] = '\0';
		return (false);
	}
	return (true);
}

bool	check_first(const char *cmd, char **token)
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
			if (!check_special_chars(cmd, i, token))
				return (false);
			else
				return (true);
		}
		i++;
	}
	return (true);
}
