/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_syntaxe_check.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:01:14 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/25 19:12:07 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

bool	check_input(const char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i] && ft_isspace(cmd[i]))
		i++;
	if (!cmd[i])
		return (false);
	return (true);
}

bool	syntax2(const char *cmd, int *status)
{
	char	*token;

	token = NULL;
	if (!check_first(cmd, &token))
	{
		ft_eprintf(MS"syntax error near unexpected token `%s'\n", token);
		free(token);
		*status = EXIT_SYNTAX_ERROR;
		return (true);
	}
	if (!check_redir(cmd, &token))
	{
		ft_eprintf(MS"syntax error near unexpected token `%s'\n", token);
		free(token);
		*status = EXIT_SYNTAX_ERROR;
		return (true);
	}
	if (!check_par(cmd, &token, 0, 0))
	{
		ft_eprintf(MS"syntax error near unexpected token `%s'\n", token);
		free(token);
		*status = EXIT_SYNTAX_ERROR;
		return (true);
	}
	return (false);
}

bool	syntax1(const char *cmd, int *status)
{
	if (!check_input(cmd))
		return (true);
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
	return (false);
}

bool	syntax_error(const char *cmd, int *status)
{
	if (syntax2(cmd, status) == true)
		return (true);
	if (syntax1(cmd, status) == true)
		return (true);
	return (false);
}
