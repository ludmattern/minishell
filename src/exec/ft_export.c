/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/11 15:56:02 by lmattern         ###   ########.fr       */
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
		name = ft_strndup(arg, equal_pos - arg);
	else
		name = ft_strdup(arg);
	is_valid = ft_isvalid_identifier(name);
	if (!is_valid)
		ft_eprintf(MS"export: `%s': not a valid identifier\n", name);
	free(name);
	return (is_valid);
}

bool	has_updated(t_env *current, char *name, char *value, bool is_local)
{
	if (ft_strncmp(current->name, name, ft_strlen(name)) == 0)
	{
		if (value && value[0])
		{
			free(current->value);
			current->value = value;
		}
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
			return (EXIT_SUCCESS);
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

int	process_export_argument(const char *arg, t_env **mini_env)
{
	char	*equal_pos;
	char	*name;
	char	*value;

	equal_pos = ft_strchr(arg, '=');
	name = NULL;
	value = NULL;
	if (equal_pos)
	{
		name = ft_strndup(arg, equal_pos - arg);
		value = ft_strdup(equal_pos + 1);
	}
	else
	{
		name = ft_strndup(arg, ft_strlen(arg));
		value = ft_strdup("");
	}
	if (name == NULL || value == NULL)
		return (free(name), free(value), EXIT_FAILURE);
	return (add_or_update_env(mini_env, name, value, false));
}

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

char	*format_env_var(const char *name, const char *value)
{
	char	*formatted_var;

	if (value && *value)
	{
		formatted_var = malloc(ft_strlen(name) + ft_strlen(value) + 15);
		if (formatted_var)
			sprintf(formatted_var, "declare -x %s=\"%s\"", name, value);
	}
	else
	{
		formatted_var = malloc(strlen(name) + ft_strlen("declare -x ") + 1);
		if (formatted_var)
			sprintf(formatted_var, "declare -x %s", name);
	}
	return (formatted_var);
}

bool	display_sorted_env(t_env *mini_env, char ***env_array)
{
	size_t	i;
	size_t	env_size;
	t_env	*current;

	env_size = ft_export_env_size(mini_env);
	if (env_size == 0)
		return (true);
	*env_array = malloc(sizeof(char *) * (env_size + 1));
	if (!*env_array)
		return (false);
	i = 0;
	current = mini_env;
	while (current)
	{
		if (!current->is_local)
		{
			(*env_array)[i] = format_env_var(current->name, current->value);
			if (!(*env_array)[i++])
				return (ft_free_double_array(*env_array), false);
		}
		current = current->next;
	}
	(*env_array)[env_size] = NULL;
	ft_sort_and_print_env(*env_array, env_size);
	return (true);
}

void	ft_sort_and_print_env(char **env, size_t size)
{
	char	*temp;
	size_t	i;
	size_t	j;

	i = -1;
	while (++i < size - 1)
	{
		j = i;
		while (++j < size)
		{
			if (ft_strncmp(env[i], env[j], ft_strlen(env[i])) > 0)
			{
				temp = env[i];
				env[i] = env[j];
				env[j] = temp;
			}
		}
	}
	i = 0;
	while (i < size)
		printf("%s\n", (env)[i++]);
	ft_free_double_array(env);
}

/*
Main export function that iterates over arguments and validates/processes them.
*/
int	ft_export(char **args, t_data *data)
{
	int	i;
	int	status;
	int	overall_status;

	data->env = NULL;
	if (!args[1])
	{
		if (!display_sorted_env(data->mini_env, &data->env))
			return (ft_free_double_array(data->env), EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	i = 0;
	overall_status = EXIT_SUCCESS;
	while (args[++i] != NULL)
	{
		if (validate_export_argument(args[i]))
		{
			status = process_export_argument(args[i], &data->mini_env);
			if (status == EXIT_FAILURE)
				overall_status = EXIT_FAILURE;
		}
		else
			overall_status = EXIT_FAILURE;
	}
	return (overall_status);
}
