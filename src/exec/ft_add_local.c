/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_local.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 19:23:50 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/03 19:53:01 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Split the name and value of the argument.
*/
int	get_local_name_n_value(char *arg, char *name, char *value)
{
	size_t  i;
	size_t  arg_len;

	i = 0;
	arg_len = ft_strlen(arg);
	while (arg[i] && arg[i] != '=')
		i++;
	name = ft_strndup(arg, i - 1);
	if (!name)
		return (EXIT_FAILURE);
	value = ft_strndup(arg + i + 1, arg_len - i);
	if (!value)
		return (free(name), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	ft_add_local(char *arg, char ***g_env, char ***l_env)
{
	char    *name;
	char    *value;

	name = NULL;
	value = NULL;
	if (get_local_name_n_value(arg, name, value) != 0)
		return (EXIT_FAILURE);
	if (find_env_index(*g_env, name) != -1)
		ft_addenv(name, value, g_env);
	else
		ft_addenv(name, value, l_env);
	return (EXIT_SUCCESS);
}