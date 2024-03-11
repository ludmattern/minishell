/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 11:25:19 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/11 18:27:14 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

int ft_cd(char **args, char **env)
{
	const char	*path;

	if (args[1] == NULL)
	{
		path = ft_getenv("HOME", env);
		if (path == NULL)
		   return (ft_eprintf("minishell: cd: HOME not set\n"), EXIT_FAILURE);
	}
	else
		path = args[1];
	if (chdir(path) != 0)
		return (perror("minishell: cd: "), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
