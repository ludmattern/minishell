/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:20:33 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/12 16:50:00 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

void	free_env(char ***env)
{
	int	i;

	i = 0;
	while ((*env) && (*env)[i])
		free((*env)[i++]);
	free(*env);
}

char	**duplicate_envp(char **envp)
{
	int		i;
	int		j;
	int		k;
	char	**minishell_env;

	i = 0;
	j = 0;
	k = 0;
	while (envp && envp[i] != NULL)
		i++;
	minishell_env = malloc((i + 1) * sizeof(char *));
	if (minishell_env == NULL) {
		perror("Failed to allocate memory for minishell_env");
		exit(EXIT_FAILURE);
	}
	while (j < i)
	{
		minishell_env[j] = malloc(strlen(envp[j]) + 1);
		if (minishell_env[j] == NULL)
		{
			perror("Failed to allocate memory for minishell_env[j]");
			while (k < j)
				free(minishell_env[k++]);
			free(minishell_env);
			exit(EXIT_FAILURE);
		}
		ft_strlcpy(minishell_env[j], envp[j], ft_strlen(envp[j]) + 1);
		j++;
	}
	minishell_env[i] = NULL;

	return (minishell_env);
}

char **ft_addenv(char *name, char *value, char ***env)
{
	int		i;
	int		j;
	char	**new_env;
	char	*new_entry;
	char	*temp;

	new_entry = (NULL);
	temp = ft_strjoin(name, "=");
	if (!temp)
		return (NULL);
	new_entry = ft_strjoin(temp, value);
	free(temp);
	if (!new_entry)
		return (NULL);
	i = -1;
	while ((*env)[++i])
	{
		if (ft_strncmp((*env)[i], name, ft_strlen(name)) == 0 && (*env)[i][ft_strlen(name)] == '=')
		{
			free((*env)[i]);
			(*env)[i] = new_entry;
			return (*env);
		}
	}
	new_env = malloc((i + 2) * sizeof(char *));
	if (!new_env)
	{
		free(new_entry);
		return (NULL);
	}
	j = -1;
	while (++j < i)
		new_env[j] = (*env)[j];
	new_env[i] = new_entry;
	new_env[i + 1] = NULL;
	free(*env);
	*env = new_env;
	return (*env);
}
