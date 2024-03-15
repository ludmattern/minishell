/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:44:15 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/14 17:13:57 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

int	main(int argc, char **argv, char **envp)
{
	char	*in_put;
	t_token	*lexed;
	t_data	*data;

	data = malloc(sizeof(t_data));
	ft_bzero(data, sizeof(t_data));
	data->env = envp;
	data->last_exit_status = EXIT_SUCCESS;
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
				free_data_structure(&data);
				//print_ast (data->ast, 0);
			}
			add_history(in_put);
			free(in_put);
		}
	}
	return (0);
}
