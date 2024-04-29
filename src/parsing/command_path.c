/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 15:43:50 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/29 17:09:17 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

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
	if (path_env == NULL)
		return (NULL);
	return (ft_split(path_env, ':'));
}

char	*build_and_verify_path(char **paths, char *temp, t_g_data *g_data)
{
	int		i;
	char	*cmd_path;
	char	*full_cmd_path;

	i = 0;
	while (paths[i] != NULL)
	{
		cmd_path = ft_strjoin(paths[i], "/");
		if (!cmd_path)
			break ;
		full_cmd_path = ft_strjoin(cmd_path, temp);
		free(cmd_path);
		if (!full_cmd_path)
			fail_exit_shell(g_data);
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
	paths = get_path_splits(path_env);
	if (!paths)
		return (temp);
	result = build_and_verify_path(paths, temp, g_data);
	if (result != NULL)
		return (result);
	free_path(paths);
	return (temp);
}

/*
char	*get_command_path(char *cmd, int i, t_g_data *g_data)
{
	t_path	p;

	i = 0;
	p.temp = ft_strdup(cmd);
	if (!p.temp)
		fail_exit_shell(g_data);
	p.path_env = getenv("PATH");
	if (p.path_env == NULL)
		return (p.temp);
	p.paths = ft_split(p.path_env, ':');
	if (p.paths == NULL)
		return (p.temp);
	while (p.paths[i] != NULL)
	{
		p.cmd_path = ft_strjoin(p.paths[i], "/");
		if (p.cmd_path == NULL)
			break ;
		p.full_cmd_path = ft_strjoin(p.cmd_path, p.temp);
		if (!p.full_cmd_path)
			fail_exit_shell(g_data);
		free(p.cmd_path);
		if (p.full_cmd_path == NULL)
			break ;
		if (access(p.full_cmd_path, X_OK) == 0)
			return (free(p.temp), free_path(p.paths), \
			free(p.paths), p.full_cmd_path);
		else
			free(p.full_cmd_path);
		i++;
	}
	return (free_path(p.paths), free(p.paths), p.temp);
}*/
