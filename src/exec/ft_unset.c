/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/15 16:15:53 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Removes the environment variable with the specified name.
*/
int	ft_unset(char *name, char ***env)
{
	size_t	i;
	size_t	j;
	size_t	name_len;

	if (!name)
		return (EXIT_SUCCESS);
	name_len = ft_strlen(name);
	i = 0;
	while ((*env) && (*env)[i])
	{
		if (ft_strncmp((*env)[i], name, name_len) == 0
			&& (*env)[i][name_len] == '=')
		{
			free((*env)[i]);
			j = i;
			while ((*env)[j])
			{
				(*env)[j] = (*env)[j + 1];
				j++;
			}
			(*env)[j - 1] = NULL;
			i--;
		}
		i++;
	}
	return(EXIT_SUCCESS);
}

/*
Removes the environment variables with the specified names.
*/
int	ft_unset_vars(char **names, char ***env)
{
	int	i;

	i = 0;
	while (names[i])
		ft_unset(names[i++], env);
	return (EXIT_SUCCESS);
}
