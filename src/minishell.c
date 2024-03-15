/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/15 17:21:40 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.h"
#include "../inc/exec.h"

/*
* Main function of the shell. It will loop until the shell is closed.
*/

void free_lexed(t_token *lexed) 
{
    t_token *tmp;

    while (lexed != NULL) 
	{
        tmp = lexed;
        lexed = lexed->next;
        free(tmp);
		
    }
}

int	main(int argc, char **argv, char **envp)
{
	char		*in_put;
	t_token		*lexed;
	t_data		*data;
	char		**global_env;
	int			last_exit_status;
	t_token		*save;

	last_exit_status = EXIT_SUCCESS;
	
	global_env = duplicate_envp(envp);
	(void)argv;
	(void)argc;

	//init ast avec data
	//init env
	while (1)
	{
		in_put = readline(">$ ");
		if (in_put[0] != '\0')
		{
			if (check_syntax(in_put) == true)
			{
				data = malloc(sizeof(t_data));
				ft_bzero(data, sizeof(t_data));
				data->env = global_env;
				data->last_exit_status = last_exit_status;
				lexed = lex_me(in_put);
				save = lexed;
				data->ast = build_ast(&lexed, data->last_exit_status);
				free_lexed(save);
				last_exit_status = run_execution(data);
				free_data_structure(&data);
			}
			add_history(in_put);
			free(in_put);
			in_put = NULL;
		}
	}
	return (0);
}
