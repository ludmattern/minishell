/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/12 17:09:24 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

void	ft_unset(char *name, char ***env)
{
	size_t	i;
	size_t	j;
	size_t	name_len;

	name_len = ft_strlen(name);
	i = 0;
	while ((*env)[i])
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
}

int	ft_unset_vars(char **names, char ***env)
{
	int	i;

	i = 0;
	while (names[i])
		ft_unset(names[i++], env);
	return (EXIT_SUCCESS);
}
