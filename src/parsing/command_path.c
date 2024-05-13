/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 15:43:50 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/13 10:29:32 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/mandatory/parse.h"

void	free_path(char **paths)
{
	int	j;

	j = 0;
	while (paths[j] != NULL)
	{
		free(paths[j]);
		j++;
	}
	free(paths);
}

char	*prepare_temp_path(char *cmd, t_g_data *g_data)
{
	char	*temp;

	temp = ft_strdup(cmd);
	if (!temp)
		fail_exit_shell(g_data);
	return (temp);
}

char	**get_path_splits(char *path_env)
{
	char	**tab;

	if (path_env == NULL)
		return (NULL);
	tab = ft_split(path_env, ':');
	if (!tab)
		free(path_env);
	return (tab);
}

char	*build_and_verify_path(char **paths, char *temp, t_g_data *g_data)
{
	int		i;
	char	*cmd_path;
	char	*full_cmd_path;

	i = 0;
	(void)g_data;
	while (paths[i] != NULL)
	{
		cmd_path = ft_strjoin(paths[i], "/");
		if (!cmd_path)
			break ;
		full_cmd_path = ft_strjoin(cmd_path, temp);
		free(cmd_path);
		if (!full_cmd_path)
			return (free_path(paths), temp);
		if (access(full_cmd_path, X_OK) == 0)
		{
			free(temp);
			free_path(paths);
			return (full_cmd_path);
		}
		free(full_cmd_path);
		i++;
	}
	return (NULL);
}

char	*get_command_path(char *cmd, t_g_data *g_data)
{
	char	*temp;
	char	*path_env;
	char	**paths;
	char	*result;

	temp = prepare_temp_path(cmd, g_data);
	path_env = ft_get_env2("PATH", g_data->mini_env, g_data);
	if (!path_env)
		return (temp);
	paths = get_path_splits(path_env);
	if (!paths)
		return (temp);
	free(path_env);
	result = build_and_verify_path(paths, temp, g_data);
	if (result != NULL)
		return (result);
	free_path(paths);
	return (temp);
}
