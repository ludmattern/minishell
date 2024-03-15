/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:56:22 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/15 11:09:08 by lmattern         ###   ########.fr       */
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
}

char	*get_command_path(char *cmd)
{
	int			i;
	char		**paths;
	char		*full_cmd_path;
	char		*cmd_path;
	char		*path_env;

	i = 0;
	path_env = getenv("PATH");
	if (path_env == NULL)
		return (cmd);
	paths = ft_split(path_env, ':');
	if (paths == NULL)
		return (cmd);
	i = 0;
	while (paths[i] != NULL)
	{
		cmd_path = ft_strjoin(paths[i], "/");
		full_cmd_path = ft_strjoin(cmd_path, cmd);
		if (access(full_cmd_path, X_OK) == 0)
			return (free_path(paths), full_cmd_path);
		i++;
	}
	free(cmd_path);
	return (free_path(paths), cmd);
}
