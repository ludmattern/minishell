/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/17 16:05:52 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.h"
#include "../inc/exec.h"

void sigint_handler(int signal) 
{
    if (signal == SIGINT) 
	{
        write(1, "\n>$ ", 4); 
    }
}

void ignore_signal() 
{
}

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

void	print_start()
{
    printf("\033[34m");
	printf("/ / /\\ \\ \\  ___ | |  ___   ___   _ __ ___    ___  | |_   ___     /\\/\\  (_) _ __  (_) ___ | |__    ___ | || |\n");
	printf("\033[35m");
	printf("\\ \\/  \\/ / / _ \\| | / __| / _ \\ | '_ ` _ \\  / _ \\ | __| / _ \\   /    \\ | || '_ \\ | |/ __|| '_ \\  / _ \\| || |\n");
	printf("\033[36m");
	printf(" \\  /\\  / |  __/| || (__ | (_) || | | | | ||  __/ | |_ | (_) | / /\\/\\ \\| || | | || |\\__ \\| | | ||  __/| || |\n");
	printf("\033[33m");
	printf("  \\/  \\/   \\___||_| \\___| \\___/ |_| |_| |_| \\___|  \\__| \\___/  \\/    \\/|_||_| |_||_||___/|_| |_| \\___||_||_| \n");
	printf("\033[37m");
}

int	main(int argc, char **argv, char **envp)
{
	char		*in_put;
	t_token		*lexed;
	t_data		*data;
	char		**global_env;
	int			last_exit_status;
	t_token		*save;
	
	print_start();
	last_exit_status = EXIT_SUCCESS;
	global_env = duplicate_envp(envp);
	(void)argv;
	(void)argc;
    signal(SIGINT, sigint_handler);
	signal(SIGQUIT, ignore_signal);
	
	while (1)
	{
		in_put = readline(">$ ");
		if (in_put == NULL) 
		{
    		write(STDOUT_FILENO, "\n", 1);  
        	break;
		}
		if (in_put[0])
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
				//free_data_structure(&data);
			}
			add_history(in_put);
			free(in_put);
			in_put = NULL;
		}
	}
	return (0);
}
