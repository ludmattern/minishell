/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   duplicate_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:20:33 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/03 13:09:09 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Copies the environment entries from source to dest.
*/
void	copy_env_entries(char **source, char **dest, int size)
{
	int	i;
	int	j;

	i = 0;
	while (i < size)
	{
		dest[i] = malloc(ft_strlen(source[i]) + 1);
		if (dest[i] == NULL)
		{
			perror("Failed to allocate memory for dest[i]");
			j = 0;
			while (j < i)
				free(dest[j++]);
			free(dest);
			exit(EXIT_FAILURE);
		}
		ft_strlcpy(dest[i], source[i], ft_strlen(source[i]) + 1);
		i++;
	}
	dest[size] = NULL;
}

/*
Allocates memory for the environment copy.
*/
char	**allocate_env_copy(int size)
{
	char	**env_copy;

	env_copy = malloc((size + 1) * sizeof(char *));
	if (env_copy == NULL)
	{
		perror("Failed to allocate memory for env_copy");
		return (NULL);
	}
	return (env_copy);
}

/*
Duplicates the environment array.
*/
char	**duplicate_envp(char **envp)
{
	int		size;
	char	**minishell_env;

	size = ft_double_array_len(envp);
	minishell_env = allocate_env_copy(size);
	copy_env_entries(envp, minishell_env, size);
	return (minishell_env);
}
