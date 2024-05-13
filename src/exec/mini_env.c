/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 12:35:33 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/13 10:28:54 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/mandatory/exec.h"

/*
Creates a new environment entry.
*/
t_env	*ft_env_new_entrie(char *name, char *value, bool is_local)
{
	t_env	*new_env_entrie;

	new_env_entrie = malloc(sizeof(t_env));
	if (!new_env_entrie)
		return (free(name), free(value), NULL);
	ft_memset(new_env_entrie, 0, sizeof(t_env));
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
