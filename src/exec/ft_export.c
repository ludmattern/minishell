/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/13 15:23:06 by lmattern         ###   ########.fr       */
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

/*
Displays the environment variables in alphabetical order when the command is 
called without arguments and adds the variables to the environment when the 
command is called with arguments
*/
int	ft_export(char **args, char ***env)
{
	int		i;
	char	*name;
	char	*value;

	i = 1;
	if (!args[i])
		return (ft_print_env_sorted(*env), EXIT_SUCCESS);
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
		{
			name = ft_strndup(args[i], ft_strchr(args[i], '=') - args[i]);
			value = ft_strdup(ft_strchr(args[i], '=') + 1);
			if (ft_addenv(name, value, env) == NULL)
				return (EXIT_FAILURE);
			free(name);
			free(value);
		}
		else
			ft_addenv(args[i], "", env);
		i++;
	}
	return (EXIT_SUCCESS);
}
