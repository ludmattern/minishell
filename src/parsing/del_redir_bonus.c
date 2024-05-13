/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_redir_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:38:57 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/13 10:23:59 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/bonus/parse_bonus.h"
#include "../../inc/bonus/exec_bonus.h"

void	toggle_quote_state(char c, int *inq, char *currentq)
{
	if (*inq && c == *currentq)
	{
		*inq = 0;
		*currentq = 0;
	}
	else if (!*inq && (c == '\'' || c == '\"'))
	{
		*inq = 1;
		*currentq = c;
	}
}

void	skip_redirections(const char *cmd, int *i)
{
	while (cmd[*i] == '>' || cmd[*i] == '<')
		(*i)++;
	while (ft_isspace((unsigned char)cmd[*i]))
		(*i)++;
}

void	skip_filenames(const char *cmd, int *i, int inq, char currentq)
{
	while (cmd[*i] != '\0' && (inq || (!ft_isspace((unsigned char) \
	cmd[*i]) && cmd[*i] != '>' && cmd[*i] != '<')))
	{
		toggle_quote_state(cmd[*i], &inq, &currentq);
		(*i)++;
	}
	while (ft_isspace((unsigned char)cmd[*i]))
		(*i)++;
}

char	*del_redir(char *cmd, int i, int j)
{
	char	*result;
	int		inq;
	char	currentq;

	inq = 0;
	currentq = 0;
	result = malloc(ft_strlen(cmd) + 1);
	if (!result)
		return (NULL);
	while (cmd[i] != '\0')
	{
		if ((cmd[i] == '\'' || cmd[i] == '\"') \
		&& (inq == 0 || currentq == cmd[i]))
			toggle_quote_state(cmd[i], &inq, &currentq);
		else if (!inq && (cmd[i] == '>' || cmd[i] == '<'))
		{
			skip_redirections(cmd, &i);
			skip_filenames(cmd, &i, inq, currentq);
			continue ;
		}
		result[j++] = cmd[i++];
	}
	result[j] = '\0';
	return (result);
}
