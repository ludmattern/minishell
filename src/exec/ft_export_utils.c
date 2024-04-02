/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/02 20:28:30 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Prints the environment variables as they are stored in the env preceded 
by "declare -x"
*/
void	print_env_var(const char *var)
{
	char	*equal_sign;

	equal_sign = strchr(var, '=');
	if (equal_sign)
	{
		printf("declare -x %.*s\"", (int)(equal_sign - var + 1), var);
		printf("%s\"\n", equal_sign + 1);
	}
	else
		printf("declare -x %s\n", var);
}

/*
Main function to print sorted environment variables
*/
void	ft_print_env_sorted(char **env)
{
	int		size;
	int		i;
	char	**env_copy;

	size = 0;
	while (env[size])
		size++;
	env_copy = copy_env(env, size);
	sort_env(env_copy, size);
	i = 0;
	while (env_copy[i])
		print_env_var(env_copy[i++]);
	free(env_copy);
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

/*
Function to sort the environment array using insertion sort
*/
void	sort_env(char **env, int size)
{
	int		i;
	int		j;
	char	*key;

	i = 1;
	while (i < size)
	{
		key = env[i];
		j = i - 1;
		while (j >= 0 && strcmp(env[j], key) > 0)
		{
			env[j + 1] = env[j];
			j = j - 1;
		}
		env[j + 1] = key;
		i++;
	}
}
