/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:20:33 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/03 11:56:59 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Updates the environment variables with the specified name and value.
*/
bool	has_updated(t_env *current, char *name, char *value, bool is_local)
{
	if (ft_strncmp(current->name, name, ft_strlen(name) + 1) == 0)
	{
		if (value && value[0])
		{
			free(current->value);
			current->value = value;
		}
		if (current->is_local == false)
			return (true);
		else
			return (current->is_local = is_local, true);
	}
	return (false);
}

/*
Processes a single export argument, assuming it's already validated.
*/
int	add_or_update_env(t_env **mini_env, char *name, char *value, bool is_local)
{
	t_env	*current;
	t_env	*last;
	t_env	*new_env = NULL;

	current = *mini_env;
	last = NULL;
	while (current != NULL)
	{
		if (has_updated(current, name, value, is_local))
		{
			free(name);
			return (EXIT_SUCCESS);
		}
		last = current;
		current = current->next;
	}
	new_env = ft_env_new_entrie(name, value, is_local);
	if (new_env == NULL)
		return (free(value), EXIT_FAILURE);
	if (last != NULL)
		last->next = new_env;
	else
		*mini_env = new_env;
	return (EXIT_SUCCESS);
}

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

bool	ft_addenv_or_update(t_env **env, char *name, char *value)
{
	t_env	*var;
	t_env	*new_var;
	char	*name2;
	char	*value2;

	value2 = NULL;
	var = find_env_var(*env, name);
	if (var)
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
	}
	else
	{
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
	}
	return (true);
}

void	ft_removeenv(t_env **env, char *name)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->name, name, ft_strlen(current->name) + 1) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->name);
			free(current->value);
			free(current);
			break ;
		}
		prev = current;
		current = current->next;
	}
}
