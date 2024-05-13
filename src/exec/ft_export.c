/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/13 10:28:00 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/mandatory/exec.h"
#include "../../inc/mandatory/parse.h"

/*
Processes a single export argument, assuming it's already validated.
*/
int	process_export_argument(const char *arg, t_env **mini_env)
{
	char	*equal_pos;
	char	*name;
	char	*value;

	equal_pos = ft_strchr(arg, '=');
	name = NULL;
	value = NULL;
	if (equal_pos)
	{
		name = ft_strndup(arg, equal_pos - arg);
		value = ft_strdup(equal_pos + 1);
	}
	else
	{
		name = ft_strndup(arg, ft_strlen(arg));
		value = ft_strdup("");
	}
	if (name == NULL || value == NULL)
		return (free(name), free(value), EXIT_FAILURE);
	return (add_or_update_env(mini_env, name, value, false));
}

/*
Main export function that iterates over arguments and validates/processes them.
*/
int	ft_export(char **args, t_data *data)
{
	int	i;
	int	status;
	int	overall_status;

	data->env = NULL;
	if (!args[1])
	{
		if (!display_sorted_env(data->mini_env, &data->env))
			return (ft_free_double_array(data->env), EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	i = 0;
	overall_status = EXIT_SUCCESS;
	while (args[++i] != NULL)
	{
		if (validate_export_argument(args[i]))
		{
			status = process_export_argument(args[i], &data->mini_env);
			if (status == EXIT_FAILURE)
				overall_status = EXIT_FAILURE;
		}
		else
			overall_status = EXIT_FAILURE;
	}
	return (overall_status);
}
