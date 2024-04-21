/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_environnement.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 16:45:34 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/21 18:39:44 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Initializes the shell variables SHLVL, PWD and OLDPWD.
*/
bool	initialize_shell_variables(t_env **mini_env)
{
	char	cwd[1024];
	t_env	*env_entry;
	char	*tmp;
	long	shell_lvl;

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
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		if(!ft_addenv_or_update(mini_env, "PWD", cwd))
			return (false);
	}
	else
	{
		if(!ft_addenv_or_update(mini_env, "PWD", ""))
			return (false);
	}
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

/*
Creates a new environment entry.
*/
t_env	*ft_env_new_entrie(char *name, char *value, bool is_local)
{
	t_env	*new_env_entrie;

	new_env_entrie = malloc(sizeof(t_env));
	if (!new_env_entrie)
		return (free(name), free(value), NULL);
	memset(new_env_entrie, 0, sizeof(t_env));
	new_env_entrie->name = name;
	new_env_entrie->value = value;
	new_env_entrie->is_local = is_local;
	new_env_entrie->next = NULL;
	return (new_env_entrie);
}

/*
Creates a new environment entry from a string.
*/
t_env	*ft_create_env_entry(char *env_str)
{
	char	*equal_pos;
	char	*name;
	char	*value;

	equal_pos = ft_strchr(env_str, '=');
	name = NULL;
	value = NULL;
	if (!equal_pos)
	{
		name = ft_strndup(env_str, ft_strlen(env_str));
	}
	else
	{
		name = ft_strndup(env_str, equal_pos - env_str);
		value = ft_strdup(equal_pos + 1);
	}
	if (!name || (equal_pos && !value))
		return (free(name), free(value), NULL);
	return (ft_env_new_entrie(name, value, false));
}

/*
Duplicates the environment array.
*/
t_env	*create_mini_env(char **envp, t_g_data **g_data)
{
	t_env	*minishell_env;
	t_env	*tmp;
	size_t	i;

	i = 0;
	minishell_env = NULL;
	while (envp && envp[i])
	{
		tmp = ft_create_env_entry(envp[i++]);
		if (!tmp)
			main_clean_exit(*g_data);
		ft_env_add_back(&minishell_env, tmp);
	}
	return (minishell_env);
}
