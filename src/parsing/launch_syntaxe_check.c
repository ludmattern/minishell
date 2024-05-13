/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_syntaxe_check.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:01:14 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/13 10:30:17 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/mandatory/parse.h"

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
	if (!check_first(cmd))
	{
		ft_eprintf(MS"syntax error near unexpected token pipe/operator\n");
		*status = EXIT_SYNTAX_ERROR;
		return (true);
	}
	if (!check_redir(cmd))
	{
		ft_eprintf(MS"syntax error near unexpected token redirection\n");
		*status = EXIT_SYNTAX_ERROR;
		return (true);
	}
	if (!check_par(cmd, 0, 0))
	{
		ft_eprintf(MS"syntax error near unexpected token parenthesis\n");
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
		ft_eprintf(MS"syntax error unclosed double quote\n");
		*status = EXIT_SYNTAX_ERROR;
		return (true);
	}
	if (!check_squotes(cmd))
	{
		ft_eprintf(MS"syntax error unclosed single quote\n");
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
