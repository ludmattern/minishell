/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/02 15:39:13 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Prints the environment variables as they are stored in the env preceded 
by "declare -x"
*/
void	print_env_var(const char *var)
{
	char	*equal_sign;

	equal_sign = strchr(var, '=');
	if (equal_sign)
	{
		printf("declare -x %.*s\"", (int)(equal_sign - var + 1), var);
		printf("%s\"\n", equal_sign + 1);
	}
	else
		printf("declare -x %s\n", var);
}

int	is_valid_env_name(const char *name)
{
	if (!ft_isalpha(*name) && *name != '_')
		return (0);
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (0);
		name++;
	}
	return (1);
}

int	process_export_arg(char *arg, char *equal_pos, char ***env)
{
	char	*name;
	char	*value;

	name = ft_strndup(arg, equal_pos - arg);
	if (!is_valid_env_name(name))
	{
		ft_eprintf("minishell: export: `%s': not a valid identifier\n", name);
		return (free(name), EXIT_FAILURE);
	}
	value = ft_strdup(equal_pos + 1);
	if (ft_addenv(name, value, env) == NULL)
		return (free(name), free(value), EXIT_FAILURE);
	free(name);
	free(value);
	return (EXIT_SUCCESS);
}

/*
Displays the environment variables in alphabetical order when the command is 
called without arguments and adds the variables to the environment when the 
command is called with arguments
*/
int	ft_export(char **args, char ***env)
{
	int		i;
	char	*equal_pos;

	if (!args[1])
		return (ft_print_env_sorted(*env), EXIT_SUCCESS);
	i = 0;
	while (args[++i])
	{
		equal_pos = ft_strchr(args[i], '=');
		if (equal_pos)
			return (process_export_arg(args[i], equal_pos, env));
		else
		{
			if (!is_valid_env_name(args[i]))
			{
				ft_eprintf("minishell: export: `%s': not a valid identifier\n",
					args[i]);
				return (EXIT_FAILURE);
			}
			ft_addenv(args[i], "", env);
		}
	}
	return (EXIT_SUCCESS);
}
