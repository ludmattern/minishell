/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/02 20:26:14 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

bool validate_export_argument(const char *arg)
{
	char	*equal_pos;
	char	*name;
	bool	is_valid;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
		name = ft_strndup(arg, equal_pos - arg);
	else
		name = ft_strdup(arg);
	is_valid = ft_isvalid_identifier(name);
	if (!is_valid)
		ft_eprintf("minishell: export: `%s': not a valid identifier\n", name);
	free(name);
	return is_valid;
}

// Processes a single export argument, assuming it's already validated.
int process_export_argument(const char *arg, char ***env)
{
	char	*equal_pos;
	char	*name;
	char	*value;
	int		status;

	equal_pos = ft_strchr(arg, '=');
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
	if (ft_addenv(name, value, env) != NULL)
		status = EXIT_SUCCESS;
	else
		status = EXIT_FAILURE;
	free(name);
	free(value);
	return status;
}

// Main export function that iterates over arguments and validates/processes them.
int ft_export(char **args, char ***env)
{
	int	i;
	int status;
	int overall_status;

    if (!args[1])
	{
        ft_print_env_sorted(*env);
        return (EXIT_SUCCESS);
    }
    overall_status = EXIT_SUCCESS;
	i = 0;
    while (args[++i] != NULL)
	{
        if (validate_export_argument(args[i]))
		{
            status = process_export_argument(args[i], env);
            if (status == EXIT_FAILURE)
                overall_status = EXIT_FAILURE;
        }
		else
            overall_status = EXIT_FAILURE;
    }

    return (overall_status);
}
