/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/05 18:53:24 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

bool	ft_isvalid_identifier(const char *name)
{
	if (!name || !*name || (!ft_isalpha(*name) && *name != '_'))
		return (false);
	name++;
	while (*name)
	{
		if (!isalnum(*name) && *name != '_')
			return (false);
		name++;
	}
	return (true);
}

/*
Removes the environment variable with the specified name.
*/
int	ft_unset(char *name, t_env **mini_env)
{
	t_env	*current;
	t_env	*prev;

	current = *mini_env;
	prev = NULL;
	if (!name || !ft_isvalid_identifier(name))
		return (ft_eprintf("minishell: unset: `%s': not a valid identifier\n",
				name), EXIT_FAILURE);
	while (current != NULL)
	{
		if (ft_strncmp(current->name, name, ft_strlen(name)) == 0)
		{
			if (prev == NULL)
				*mini_env = current->next;
			else
				prev->next = current->next;
			free(current->name);
			free(current->value);
			free(current);
			return (EXIT_SUCCESS);
		}
		prev = current;
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

/*
Removes the environment variables with the specified names.
*/
int	ft_unset_vars(char **names, t_env **mini_env)
{
	int	i;
	int	status;

	i = 1;
	status = EXIT_SUCCESS;
	while (names[i])
	{
		if (ft_unset(names[i++], mini_env) == EXIT_FAILURE)
			status = EXIT_FAILURE;
	}
	return (status);
}
