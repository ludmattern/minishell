/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/02 21:04:47 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.h"
#include "../inc/exec.h"

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

void sigint_handler(int signal) 
{
	char *path = getcwd(NULL, 0);
	char *join = ft_strjoin(path, " $> ");
    if (signal == SIGINT) 
	{
        printf("\n%s", join); 
    }
	free(join);
	free(path);
}

void ignore_signal() 
{
}

void free_lexed(t_token *lexed) 
{
    t_token	*tmp;

    while (lexed != NULL) 
	{
        tmp = lexed;	
		lexed = lexed->next;	
		free(tmp->value);
		tmp->value = NULL;
		free(tmp);
		tmp = NULL;
    }
}

void lex_mallox_error(t_token *lex)
{
	free_lexed(lex);
	printf("mallox error");
	exit(EXIT_FAILURE);
}


void free_parsing(t_node *ast,  t_token *lex)
{
	free_lexed(lex);
	free_tree(ast);
	printf("mallox error");
	exit(EXIT_FAILURE);
}

void initialize_shell_variables_with_export(char ***env) {
    char cwd[1024];
    int shlvl;
    char *shlvlStr, *shlvlCmd, *tempStr;

    // Set PWD
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        char *pwdCmd = ft_strjoin("PWD=", cwd); // Assumes ft_strjoin allocates memory for the result
        ft_export((char *[]){ "export", pwdCmd, NULL }, env);
        free(pwdCmd); // Free the memory allocated by ft_strjoin
    }
	ft_export((char *[]){ "export", "OLDPWD", NULL }, env);
    // Handle SHLVL
    // getenv might not be directly usable if you're not using the actual process environment. Adjust as necessary.
    shlvlStr = getenv("SHLVL");
    shlvl = shlvlStr ? ft_atoi(shlvlStr) + 1 : 1;

    tempStr = ft_itoa(shlvl); // Convert integer to string
    shlvlCmd = ft_strjoin("SHLVL=", tempStr); // Concatenate "SHLVL=" with the SHLVL value
    free(tempStr); // Free the temporary string created by ft_itoa

    ft_export((char *[]){ "export", shlvlCmd, NULL }, env);
    free(shlvlCmd); // Free the memory allocated by ft_strjoin
}



int	main(int argc, char **argv, char **envp)
{
	char		*in_put;
	t_token		*lexed;
	t_data		*data;
	char		**global_env;
	int			i;
	int			last_exit_status;
	t_token		*save;
	char *join;
	char *path;

	//print_start();
	last_exit_status = EXIT_SUCCESS;
	global_env = duplicate_envp(envp);
	initialize_shell_variables_with_export(&global_env);
	(void)argv;
	(void)argc;
    signal(SIGINT, sigint_handler);
	while (1)
	{
		path = getcwd(NULL, 0);
		join = ft_strjoin(path, " $> ");
		in_put = readline(join);
		free(join);
		free(path);
		if (in_put == NULL) 
		{
    		write(STDOUT_FILENO, "\n", 1);  
        	break;
		}
		if (in_put[0])
		{
			if (check_syntax(in_put) == false)
				last_exit_status = EXIT_GENERAL_ERROR;
			else
			{
				data = malloc(sizeof(t_data));
				//if (!data)
				//	return (printf("MALLOC ERROR"));
				ft_bzero(data, sizeof(t_data));
				data->env = global_env;
				data->last_exit_status = last_exit_status;
				lexed = lex_me(in_put);
				if (lexed->error == -1)
					lex_mallox_error(lexed);
				save = lexed;
				data->ast = build_ast(&lexed, data->last_exit_status);
				//if (!data->ast)
				//	free_parsing(data->ast, lexed);
				free_lexed(save);
				last_exit_status = run_execution(data);
				global_env = data->env;
				free_data_structure(&data);
			}
			add_history(in_put);
			free(in_put);
			in_put = NULL;
		}
	}
	i = 0;
	while (global_env[i])
		free(global_env[i++]);
	free(global_env);
	return (0);
}
