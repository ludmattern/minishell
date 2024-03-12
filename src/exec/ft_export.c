/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/12 17:12:55 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

void print_env_var(const char *var)
{
    char *equal_sign = strchr(var, '=');
    if (equal_sign)
	{
        printf("declare -x %.*s\"", (int)(equal_sign - var + 1), var);
        printf("%s\"\n", equal_sign + 1);
    }
	else
	{
        printf("declare -x %s\n", var);
    }
}

void ft_print_env_sorted(char **env)
{
    int i, j;
    char *key;

    for (i = 0; env[i]; i++);
    char **env_copy = malloc(sizeof(char *) * (i + 1));
    for (j = 0; j < i; j++)
	{
        env_copy[j] = env[j];
    }
    env_copy[i] = NULL; // Marqueur de fin
    for (i = 1; env_copy[i]; i++)
	{
        key = env_copy[i];
        j = i - 1;

        while (j >= 0 && strcmp(env_copy[j], key) > 0)
		{
            env_copy[j + 1] = env_copy[j];
            j = j - 1;
        }
        env_copy[j + 1] = key;
    }
    for (i = 0; env_copy[i]; i++)
	{
        print_env_var(env_copy[i]);
    }
    free(env_copy);
}

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