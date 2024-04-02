/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/02 19:08:31 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

bool	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

long long	ft_strtol(const char *str, char *endptr, bool *error)
{
	long long	res;
	int			sign;
	int			digit;

	res = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		digit = *str - '0';
		if (sign == 1 && (LLONG_MAX - digit) / 10 < res)
			return (*error = true, LLONG_MAX);
		else if (sign == -1 && (LLONG_MIN + digit) / 10 > -res)
			return (*error = true, LLONG_MIN);
		res = res * 10 + digit;
		str++;
	}
	return (*endptr = *str, res * sign);
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

int	ft_exit(char **args, t_data **data)
{
	long long	status;
	char		endptr;
	bool		error;

	error = false;
	if (args[1] && !args[2])
	{
		status = ft_strtol(args[1], &endptr, &error);
		if (endptr != '\0' || error == true)
		{
			printf("exit\n");
			ft_eprintf("minishell: exit: %s: numeric argument required\n",
				args[1]);
			exit(EXIT_SYNTAX_ERROR);
		}
		free_data_structure(data);
		exit((int)(status % 256 + 256) % 256);
	}
	else if (args[1] && args[2])
		return (printf("exit\n"),
			ft_eprintf("minishell: exit: too many arguments\n"),
			EXIT_GENERAL_ERROR);
	free_data_structure(data);
	printf("exit\n");
	exit(EXIT_SUCCESS);
}
