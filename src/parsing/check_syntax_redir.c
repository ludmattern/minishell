/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_redir.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:59:09 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/19 16:01:06 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

void	toggle_quo(bool *in_quotes, char c, int i, const char *cmd)
{
	if ((c == '\'' || c == '"') && (!*in_quotes || \
	(*in_quotes && cmd[i - 1] != '\\')))
	{
		*in_quotes = !*in_quotes;
	}
}

bool	validate_special_char(const char *cmd, int *i, char **token)
{
	int	j;

	j = *i + 1;
	while (cmd[j] == ' ')
		j++;
	if (cmd[j] == '\0' || strchr("|&<>", cmd[j]))
	{
		if (cmd[j] == '\0')
			*token = ft_strdup("newline");
		else
		{
			*token = malloc(sizeof(char) * 3);
			if (!*token)
				return (perror("malloc"), false);
			(*token)[0] = cmd[j];
			if (cmd[j + 1] == cmd[j])
				(*token)[1] = cmd[j];
			else
				(*token)[1] = '\0';
			(*token)[2] = '\0';
		}
		return (false);
	}
	*i = j;
	return (true);
}

bool	check_redir(const char *cmd, char **token)
{
	bool	in_quotes;
	int		i;

	in_quotes = false;
	i = 0;
	while (cmd[i] != '\0')
	{
		toggle_quo(&in_quotes, cmd[i], i, cmd);
		if (!in_quotes)
		{
			if (cmd[i] == '>' || cmd[i] == '<' || \
			cmd[i] == '|' || cmd[i] == '&')
			{
				if (cmd[i + 1] == cmd[i])
					i++;
				if (!validate_special_char(cmd, &i, token))
					return (false);
			}
		}
		i++;
	}
	return (true);
}