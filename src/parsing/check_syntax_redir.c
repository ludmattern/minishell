/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_redir.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:59:09 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/13 10:29:11 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/mandatory/parse.h"

void	toggle_quo(bool *in_quotes, char c, int i, const char *cmd)
{
	if ((c == '\'' || c == '"') && (!*in_quotes || \
	(*in_quotes && cmd[i - 1] != '\\')))
	{
		*in_quotes = !*in_quotes;
	}
}

bool	validate_special_char(const char *cmd, int *i)
{
	int	j;
	int	k;

	j = *i + 1;
	while (ft_isspace(cmd[j]))
		j++;
	if (cmd[j] == '\0' || ft_strchr("|&", cmd[j]))
		return (false);
	if (cmd[*i] == '>' || cmd[*i] == '<')
	{
		if (cmd[j] == '>' || cmd[j] == '<')
			return (false);
		k = j;
		while (cmd[k] && !ft_strchr("><&|(", cmd[k]))
			k++;
		if (cmd[k] == '(')
			return (false);
	}
	return (true);
}

bool	check_redir(const char *cmd)
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
				if (!validate_special_char(cmd, &i))
					return (false);
			}
		}
		i++;
	}
	return (true);
}
