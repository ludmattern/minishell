/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_local.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 19:23:50 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/03 13:28:55 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
split the name and value of the argument.
*/
int	get_local_name_n_value(char *arg, char *name, char *value)
{
	size_t	i;
	size_t	arg_len;

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

int	ft_add_local(char *arg, t_env **mini_env)
{
	char	*equal_pos;
	char	*name;
	char	*value;

	equal_pos = ft_strchr(arg, '=');
	name = NULL;
	value = NULL;
	name = ft_strndup(arg, equal_pos - arg);
	value = ft_strdup(equal_pos + 1);
	if (name == NULL || value == NULL)
		return (free(name), free(value), EXIT_FAILURE);
	return (add_or_update_env(mini_env, name, value, true));
}
