/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:20:33 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/05 17:37:07 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Finds the index of the environment variable with the specified name.
*/
t_env	*find_env_var(t_env *env, const char *name)
{
	while (env)
	{
		if (strcmp(env->name, name) == 0)
			return env;
		env = env->next;
	}
	return NULL;
}

void	ft_addenv_or_update(t_env **env, const char *name, const char *value)
{
	t_env *var = find_env_var(*env, name);
	if (var)
	{
		free(var->value); // Free old value
		var->value = strdup(value); // Set new value
	}
	else
	{
		t_env *new_var = ft_env_new_entrie(strdup(name), strdup(value), false);
		if (new_var)
			ft_env_add_back(env, new_var); // Add to the end of the list
	}
}

void	ft_removeenv(t_env **env, const char *name) {
	t_env *current = *env;
	t_env *prev = NULL;
	while (current)
	{
		if (strcmp(current->name, name) == 0)
		{
			if (prev)
			{
				prev->next = current->next;
			} else
			{
				*env = current->next; // Remove first node
			}
			free(current->name);
			free(current->value);
			free(current);
			break;
		}
		prev = current;
		current = current->next;
	}
}
