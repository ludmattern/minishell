/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:20:33 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/03 18:10:46 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Frees the environment array.
*/
void	free_env(char ***env)
{
	int	i;

	i = 0;
	while ((*env) && (*env)[i])
		free((*env)[i++]);
	free(*env);
}

/*
Finds the index of the environment variable with the specified name.
*/
int	find_env_index(char **env, const char *name)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], name, ft_strlen(name)) == 0
			&& env[i][ft_strlen(name)] == '=')
			return (i);
		i++;
	}
	return (-1);
}

/*
Creates a new environment entry.
*/
char	*create_env_entry(const char *name, const char *value)
{
	char	*temp;
	char	*new_entry;

	temp = ft_strjoin(name, "=");
	if (!temp)
		return (NULL);
	new_entry = ft_strjoin(temp, value);
	free(temp);
	return (new_entry);
}

/*
Updates the environment array.
*/
char	**update_env(char **env, char *new_entry, int index)
{
	char	**new_env;
	int		size;
	int		i;

	if (index >= 0)
	{
		free(env[index]);
		env[index] = new_entry;
	}
	else
	{
		size = count_env_entries(env);
		new_env = malloc((size + 2) * sizeof(char *));
		if (!new_env)
			return (free(new_entry), NULL);
		i = -1;
		while (++i < size)
			new_env[i] = env[i];
		new_env[size] = new_entry;
		new_env[size + 1] = NULL;
		free(env);
		env = new_env;
	}
	return (env);
}

/*
Adds a new environment variable.
*/
char	**ft_addenv(char *name, char *value, char ***env)
{
	char	*new_entry;
	int		index;

	new_entry = create_env_entry(name, value);
	if (!new_entry)
		return (NULL);
	index = find_env_index(*env, name);
	*env = update_env(*env, new_entry, index);
	return (*env);
}
