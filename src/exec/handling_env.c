/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:20:33 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/19 14:04:34 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Updates the environment variables with the specified name and value.
*/
bool	has_updated(t_env *current, char *name, char *value, bool is_local)
{
	if (ft_strncmp(current->name, name, ft_strlen(name)) == 0)
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
	t_env	*new_env;

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

	tmp = env;
	while (tmp)
	{
		if (strcmp(tmp->name, name) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	ft_addenv_or_update(t_env **env, char *name, char *value)
{
	t_env	*var;
	t_env	*new_var;

	var = find_env_var(*env, name);
	if (var)
	{
		ft_free(var->value);
		if (value && *value)
			var->value = ft_strdup(value);
		else
			var->value = NULL;
	}
	else
	{
		new_var = ft_env_new_entrie(strdup(name), ft_strdup(value), false);
		if (new_var)
			ft_env_add_back(env, new_var);
	}
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
