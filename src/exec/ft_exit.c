/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/02 14:50:57 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

bool	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

long long	ft_atoll(const char *str)
{
	long long	res;
	int			sign;

	res = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
		sign = (*str++ == '-') ? -1 : 1;
	while (ft_isdigit(*str))
		res = res * 10 + (*str++ - '0');
	return (res * sign);
}

bool	ft_isnumber(const char *str)
{
	if (*str == '-' || *str == '+')
		str++;
	if (!*str)
		return (false);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

int	ft_exit(char **args)
{
	long long status;

	if (args[1] && !args[2])
	{
		if (!ft_isnumber(args[1]))
		{
			ft_eprintf("minishell: exit: %s: numeric argument required\n",
					args[1]);
			exit(EXIT_SYNTAX_ERROR);
		}
		status = ft_atoll(args[1]);
		exit((int)(status % 256 + 256) % 256);
	}
	else if (args[1] && args[2])
	{
		ft_eprintf("minishell: exit: too many arguments\n");
		return (EXIT_GENERAL_ERROR);
	}
	exit(EXIT_SUCCESS);
}
