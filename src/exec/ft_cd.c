/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 11:25:19 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/15 16:43:47 by lmattern         ###   ########.fr       */
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

/*
Changes the current directory to the one specified in the arguments.
*/
int	ft_cd(char **args, t_env **env)
{
	const char	*path;
	char		oldpwd[1024];

	oldpwd[0] = '\0';
	if (!getcwd(oldpwd, sizeof(oldpwd)))
		perror(MS"error retrieving current directory");
	if (args[1] != NULL && args[2] != NULL)
		return (ft_eprintf(MS"cd: too many arguments\n"),
			EXIT_FAILURE);
	if (args[1] == NULL)
	{
		path = ft_get_env("HOME", *env);
		if (path == NULL)
			return (ft_eprintf(MS"cd: HOME not set\n"), EXIT_FAILURE);
	}
	else
		path = args[1];
	if (chdir(path) != 0)
		return(ft_eprintf(MS"cd: %s: %s\n", path, strerror(errno)), EXIT_FAILURE);
	cd_update_env(oldpwd, env);
	return (EXIT_SUCCESS);
}
