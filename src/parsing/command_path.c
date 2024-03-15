/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:56:22 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/15 16:10:58 by fprevot          ###   ########.fr       */
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

char *get_command_path(char *cmd)
{
    char **paths;
    char *full_cmd_path;
    char *cmd_path;
    char *path_env;
    int i;
	
	i = 0;
    path_env = getenv("PATH");
    if (path_env == NULL) 
		return (cmd);
    paths = ft_split(path_env, ':');
    if (paths == NULL) 
		return (cmd);
    while (paths[i] != NULL)
	{
        cmd_path = ft_strjoin(paths[i], "/");
        if (cmd_path == NULL) 
			break;
        full_cmd_path = ft_strjoin(cmd_path, cmd);
        free(cmd_path);
        if (full_cmd_path == NULL)
			break;
        if (access(full_cmd_path, X_OK) == 0)
            return (free_path(paths), free(paths), full_cmd_path);
		else 
            free(full_cmd_path);
        i++;
    }
    return (free_path(paths), free(paths), cmd);
}

                