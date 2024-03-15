/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/13 14:40:12 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Main function to print sorted environment variables
*/
void	ft_print_env_sorted(char **env)
{
	int		size;
	char	**env_copy;

	size = 0;
	while (env[size])
		size++;
	env_copy = copy_env(env, size);
	sort_env(env_copy, size);
	print_and_free_env(env_copy);
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

/*
Function to print and free the environment array
*/
void	print_and_free_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		print_env_var(env[i++]);
	free(env);
}
