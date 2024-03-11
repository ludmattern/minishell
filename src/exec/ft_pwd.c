/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 11:25:19 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/11 18:32:29 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

int ft_pwd(char **args)
{
    char *cwd;

    if (args[1])
        return (ft_eprintf("minishell: pwd: too many arguments\n"), EXIT_FAILURE);
    cwd = getcwd(NULL, 0);
    if (cwd != NULL)
    {
        printf("%s\n", cwd);
        free(cwd);
    }
	else
	{
        perror("getcwd() error");
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
