/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_environnement.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 16:45:34 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/08 16:07:40 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Initializes the shell level variable.
*/
bool	init_shell_level(t_env **mini_env)
{
	char	*tmp;
	long	shell_lvl;
	t_env	*env_entry;

	env_entry = find_env_var(*mini_env, "SHLVL");
	shell_lvl = 1;
	if (env_entry && env_entry->value)
		shell_lvl = ft_atoi(env_entry->value) + 1;
	tmp = ft_itoa((int)shell_lvl);
	if (!tmp)
		return (false);
	if (!ft_addenv_or_update(mini_env, "SHLVL", tmp))
		return (false);
	free(tmp);
	return (true);
}

/*
Initializes the PWD variable.
*/
bool	init_cwd(t_env **mini_env)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		if (!ft_addenv_or_update(mini_env, "PWD", cwd))
			return (false);
	}
	else
	{
		if (!ft_addenv_or_update(mini_env, "PWD", ""))
			return (false);
	}
	return (true);
}

/*
Initializes the shell variables SHLVL, PWD and OLDPWD.
*/
bool	initialize_shell_variables(t_env **mini_env)
{
	t_env	*env_entry;
	char	*tmp;

	if (!init_shell_level(mini_env))
		return (false);
	if (!init_cwd(mini_env))
		return (false);
	env_entry = find_env_var(*mini_env, "OLDPWD");
	if (env_entry && env_entry->value)
	{
		tmp = ft_strdup(env_entry->value);
		if (!ft_addenv_or_update(mini_env, "OLDPWD", tmp))
			return (free(tmp), false);
		free(tmp);
	}
	else
	{
		if (!ft_addenv_or_update(mini_env, "OLDPWD", ""))
			return (false);
	}
	return (true);
}
