/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/13 15:23:30 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Prints the environment variables unless there are too many arguments.
*/
int	ft_env(char **args, char ***env)
{
	size_t	i;

	if (args[1])
		return (ft_eprintf("minishell: env: too many args\n"), EXIT_FAILURE);
	i = 0;
	while ((*env) && (*env)[i])
		printf("%s\n", (*env)[i++]);
	return (EXIT_SUCCESS);
}
