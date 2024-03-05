/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/05 15:27:08 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
* Main function of the shell. It will loop until the shell is closed.
*/
int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (data == NULL)
		return (1);
	(void)argc;
	(void)argv;
	(void)envp;
	ft_bzero(data, sizeof(t_data));
	data->env = envp;
	data->ast = create_ast();
	data->last_exit_status = EXIT_SUCCESS;
	run_execution(data);
	return (data->last_exit_status);
}
