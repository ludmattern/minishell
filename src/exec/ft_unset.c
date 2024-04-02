/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/02 19:34:12 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

bool ft_isvalid_identifier(const char *name)
{
	if (!name || !*name || (!ft_isalpha(*name) && *name != '_'))
		return (false);
	name++;
	while (*name)
	{
		if (!isalnum(*name) && *name != '_')
			return (false);
		name++;
	}
	return (true);
}

/*
Removes the environment variable with the specified name.
*/
int	ft_unset(char *name, char ***env)
{
	size_t	i;
	size_t	j;
	size_t	name_len;

	if (!name || !ft_isvalid_identifier(name))
		return (ft_eprintf("minishell: unset: `%s': not a valid identifier\n", name), EXIT_FAILURE);
	name_len = ft_strlen(name);
	i = -1;
	while ((*env) && (*env)[++i])
	{
		if (ft_strncmp((*env)[i], name, name_len) == 0
			&& (*env)[i][name_len] == '=')
		{
			free((*env)[i]);
			j = i - 1;
			while ((*env)[++j])
				(*env)[j] = (*env)[j + 1];
			(*env)[j - 1] = NULL;
			i--;
		}
	}
	return (EXIT_SUCCESS);
}

/*
Removes the environment variables with the specified names.
*/
int	ft_unset_vars(char **names, char ***env)
{
	int	i;
	int status;

	i = 1;
	status = EXIT_SUCCESS;
	while (names[i])
	{
		if (ft_unset(names[i++], env) == EXIT_FAILURE)
			status = EXIT_FAILURE;
	}
	return (status);
}
