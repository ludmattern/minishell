/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation_env_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:20:33 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/04 12:37:29 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Finds the index of the environment variable with the specified name.
*/
t_env	*find_env_var(t_env *env, char *name)
{
	t_env	*tmp;
	size_t	len;

	tmp = env;
	while (tmp)
	{
		len = ft_strlen(tmp->name) + 1;
		if (ft_strncmp(tmp->name, name, len) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

bool	ft_update_env(t_env	*var, char *value)
{
	ft_free(var->value);
	if (value && *value)
	{
		var->value = ft_strdup(value);
		if (!var->value)
			return (false);
	}
	else
		var->value = NULL;
	return (true);
}

bool	ft_add_env(t_env **env, char *name, char *value)
{
	char	*name2;
	char	*value2;
	t_env	*new_var;

	value2 = NULL;
	name2 = ft_strdup(name);
	if (!name2)
		return (false);
	if (value && *value)
	{
		value2 = ft_strdup(value);
		if (!value2)
			return (ft_free(name2), false);
	}
	new_var = ft_env_new_entrie(name2, value2, false);
	if (!new_var)
		return (ft_free(name2), ft_free(value2), false);
	ft_env_add_back(env, new_var);
	return (true);
}

bool	ft_addenv_or_update(t_env **env, char *name, char *value)
{
	t_env	*var;

	var = find_env_var(*env, name);
	if (var)
	{
		if (!ft_update_env(var, value))
			return (false);
	}
	else
	{
		if (!ft_add_env(env, name, value))
			return (false);
	}
	return (true);
}
