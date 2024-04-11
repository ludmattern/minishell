/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_environnement.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 16:45:34 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/09 13:59:24 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

void	env_err(t_env *mini_env, char *name, char *value)
{
	if (mini_env)
		free_mini_env(mini_env);
	if (name)
		free(name);
	if (value)
		free(value);
	exit(EXIT_FAILURE);
}

void	free_mini_env(t_env *mini_env)
{
	t_env	*tmp;

	while (mini_env)
	{
		tmp = mini_env;
		mini_env = mini_env->next;
		free(tmp->name);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

void	initialize_shell_variables(t_env **mini_env)
{
	char	cwd[1024];
	t_env	*env_entry;
	char	*tmp;
	long	shell_lvl;

	env_entry = find_env_var(*mini_env, "SHLVL");
	if (env_entry && env_entry->value)
		shell_lvl = ft_atoi(env_entry->value) + 1;
	else
		shell_lvl = 1;
	tmp = ft_itoa((int)shell_lvl);
	ft_addenv_or_update(mini_env, "SHLVL", tmp);
	free(tmp);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_addenv_or_update(mini_env, "PWD", cwd);
	env_entry = find_env_var(*mini_env, "OLDPWD");
	if (!env_entry)
		ft_addenv_or_update(mini_env, "OLDPWD", "");
}

t_env	*ft_env_last(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_env_add_back(t_env **lst, t_env *new)
{
	t_env	*last_node;

	if (!new)
		return ;
	if (!*lst)
		*lst = new;
	else
	{
		last_node = ft_env_last(*lst);
		last_node->next = new;
	}
}

t_env	*ft_env_new_entrie(char *name, char *value, bool is_local)
{
	t_env	*new_env_entrie;

	new_env_entrie = malloc(sizeof(t_env));
	if (!new_env_entrie)
		return (NULL);
	memset(new_env_entrie, 0, sizeof(t_env));
	new_env_entrie->name = name;
	new_env_entrie->value = value;
	new_env_entrie->is_local = is_local;
	return (new_env_entrie);
}

t_env	*ft_create_env_entry(const char *env_str)
{
	char	*equal_pos;
	char	*name;
	char	*value;

	equal_pos = ft_strchr(env_str, '=');
	name = NULL;
	value = NULL;
	if (!equal_pos)
		name = ft_strndup(env_str, ft_strlen(env_str));
	else
	{
		name = ft_strndup(env_str, equal_pos - env_str);
		value = ft_strdup(equal_pos + 1);
	}
	if (!name || (equal_pos && !value))
		return (free(name), free(value), NULL);
	if (name && ft_strlen(name) == 1 && name[0] == '_')
		return (ft_env_new_entrie(name, value, true));
	return (ft_env_new_entrie(name, value, false));
}

/*
Duplicates the environment array.
*/
t_env	*create_mini_env(char **envp)
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
		{
			free_mini_env(minishell_env);
			exit (EXIT_GENERAL_ERROR);
		}
		ft_env_add_back(&minishell_env, tmp);
	}
	return (minishell_env);
}

t_g_data	initialize_environnement(char **envp)
{
	t_g_data	g_data;

	memset(&g_data, 0, sizeof(t_g_data));
	g_data.last_exit_status = EXIT_SUCCESS;
	g_data.mini_env = create_mini_env(envp);
	initialize_shell_variables(&g_data.mini_env);
	return (g_data);
}
