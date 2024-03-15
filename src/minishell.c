/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/15 11:57:48 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.h"
#include "../inc/exec.h"

/*
* Main function of the shell. It will loop until the shell is closed.
*/
int	main(int argc, char **argv, char **envp)
{
	char		*in_put;
	t_token		*lexed;
	t_data		*data;
	char		**global_env;
	int			last_exit_status;

	data = malloc(sizeof(t_data));
	ft_bzero(data, sizeof(t_data));
	data->env = envp;
	last_exit_status = EXIT_SUCCESS;
	data->last_exit_status = last_exit_status;
	global_env = duplicate_envp(envp);
	data->env = global_env;
	(void)argv;
	(void)argc;
	while (1)
	{
		in_put = readline(">$ ");
		if (in_put[0] != '\0')
		{
			if (check_syntax(in_put) == true)
			{
				lexed = lex_me(in_put);
				data->ast = build_ast(&lexed, data->last_exit_status);
				last_exit_status = run_execution(data);
			}
			add_history(in_put);
			free(in_put);
		}
	}
	return (0);
}
