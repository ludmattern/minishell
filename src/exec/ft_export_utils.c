/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/05 10:10:02 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Function to copy the environment array
*/
char	**copy_env(char **env, int size)
{
	char	**env_copy;
	int		i;

	i = 0;
	env_copy = malloc(sizeof(char *) * (size + 1));
	if (env_copy == NULL)
		return (perror("Failed to allocate memory for env_copy"), NULL);
	while (i < size)
	{
		env_copy[i] = env[i];
		i++;
	}
	env_copy[size] = NULL;
	return (env_copy);
}
