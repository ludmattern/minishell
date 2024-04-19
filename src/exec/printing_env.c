/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/19 13:16:48 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Validates a single export argument.
*/
char	*format_env_var(const char *name, const char *value)
{
	char	*formatted_var;

	if (value && *value)
	{
		formatted_var = ft_calloc(sizeof(char), ft_strlen(name) + \
			ft_strlen(value) + 15);
		if (formatted_var)
			ft_sprintf(formatted_var, "declare -x %s=\"%s\"", name, value);
	}
	else
	{
		formatted_var = ft_calloc(sizeof(char), ft_strlen(name) + \
			ft_strlen("declare -x ") + 1);
		if (formatted_var)
			ft_sprintf(formatted_var, "declare -x %s", name);
	}
	return (formatted_var);
}

/*
Displays the sorted environment variables.
*/
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

/*
Sorts and prints the environment variables.
*/
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
