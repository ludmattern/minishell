/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:10:35 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/05 15:13:14 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Prints the environment variables unless there are too many arguments.
*/
int	ft_env(char **args, t_env *mini_env)
{
	t_env	*tmp;

	if (args[1])
		return (ft_eprintf("minishell: env: too many args\n"), EXIT_FAILURE);
	tmp = mini_env;
	while (tmp)
	{
		if (tmp->name && ft_strlen(tmp->name) == 1 && tmp->name[0] == '_')
		{
			if (tmp->value)
				ft_printf("%s=%s\n", tmp->name, tmp->value);
			else
				ft_printf("%s\n", tmp->name);
		}
		if (tmp->is_local)
		{
			tmp = tmp->next;
			continue ;
		}
		else if (tmp->value && tmp->value[0])
			ft_printf("%s=%s\n", tmp->name, tmp->value);
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}
