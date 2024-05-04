/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/04 12:37:20 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Validates a single export argument.
*/
bool	validate_export_argument(const char *arg)
{
	char	*equal_pos;
	char	*name;
	bool	is_valid;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
	{
		if (equal_pos == arg)
		{
			ft_eprintf(MS"export: `%s': not a valid identifier\n", arg);
			return (false);
		}
		name = ft_strndup(arg, equal_pos - arg);
	}
	else
		name = ft_strdup(arg);
	is_valid = ft_isvalid_identifier(name);
	if (!is_valid)
		ft_eprintf(MS"export: `%s': not a valid identifier\n", name);
	free(name);
	return (is_valid);
}

/*
Function to get the size of the exported environment array
*/
size_t	ft_export_env_size(t_env *lst)
{
	int	i;

	if (!lst)
		return (0);
	i = 0;
	while (lst)
	{
		if (lst->is_local == false)
			i++;
		lst = lst->next;
	}
	return (i);
}

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
