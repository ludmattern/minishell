/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 11:25:19 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/21 16:35:13 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Updates the environment variables PWD and OLDPWD.
*/
void	cd_update_env(char *oldpwd, t_env **env)
{
	char	newpwd[1024];

	if (oldpwd[0] != '\0')
		ft_addenv_or_update(env, "OLDPWD", oldpwd);
	if (getcwd(newpwd, sizeof(newpwd)) != NULL)
		ft_addenv_or_update(env, "PWD", newpwd);
	else
		perror(MS"error updating PWD");
}

void	init_ft_cd(char *oldpwd, bool *alloc)
{
	oldpwd[0] = '\0';
	*alloc = false;
}

bool	cd_to_home(char **args, t_env **env, char **path, bool *alloc)
{
	if (args[1] == NULL)
	{
		*path = ft_get_env("HOME", *env);
		*alloc = true;
		if (path == NULL)
		{
			ft_eprintf(MS"cd: HOME not set\n");
			return (false);
		}
	}
	else
		*path = args[1];
	return (true);
}

/*
Changes the current directory to the one specified in the arguments.
*/
int	ft_cd(char **args, t_env **env)
{
	const char	*path;
	char		oldpwd[1024];
	bool		alloc;

	path = NULL;
	init_ft_cd(oldpwd, &alloc);
	if (!getcwd(oldpwd, sizeof(oldpwd)))
		perror(MS"error retrieving current directory");
	if (args[1] != NULL && args[2] != NULL)
		return (ft_eprintf(MS"cd: too many arguments\n"),
			EXIT_FAILURE);
	if (!cd_to_home(args, env, (char **)&path, &alloc))
		return (EXIT_FAILURE);
	if (chdir(path) != 0)
	{
		ft_eprintf(MS"cd: %s: %s\n", path, strerror(errno));
		if (alloc)
			free((char *)path);
		return (EXIT_FAILURE);
	}
	if (alloc)
		free((char *)path);
	cd_update_env(oldpwd, env);
	return (EXIT_SUCCESS);
}
