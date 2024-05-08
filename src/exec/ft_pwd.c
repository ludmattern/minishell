/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 11:25:19 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/08 18:10:16 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Prints the current working directory.
*/
int	ft_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		printf("%s\n", cwd);
		free(cwd);
	}
	else
	{
		ft_eprintf("pwd: error retrieving current directory: ");
		perror("getcwd: cannot access parent directories");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
