/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/03 13:18:33 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.h"
#include "../inc/exec.h"

void	print_start(void)
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

void	sigint_handler(int signal)
{
	char	*path;
	char	*join;

	path = getcwd(NULL, 0);
	join = ft_strjoin(path, " $> ");
	if (signal == SIGINT)
		printf("\n%s", join);
	free(join);
	free(path);
}

void	ignore_signal(void)
{
}

void	free_lexed(t_token *lexed)
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

void	lex_mallox_error(t_token *lex)
{
	free_lexed(lex);
	printf("mallox error");
	exit(EXIT_FAILURE);
}

void	free_parsing(t_node *ast, t_token *lex)
{
	free_lexed(lex);
	free_tree(ast);
	printf("mallox error");
	exit(EXIT_FAILURE);
}

void	initialize_shell_variables_with_export(char ***env)
{
	char	cwd[1024];
	int		shell_lvl;
	char	*shell_lvl_str;
	char	*shell_lvl_cmd;
	char	*tmp;
	char	*pwd_cmd;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		pwd_cmd = ft_strjoin("PWD=", cwd);
		ft_export((char *[]){ "export", pwd_cmd, NULL }, env);
		free(pwd_cmd);
	}
	ft_export((char *[]){ "export", "OLDPWD", NULL }, env);
	shell_lvl_str = getenv("SHLVL");
	if (shell_lvl_str)
		shell_lvl = ft_atoi(shell_lvl_str) + 1;
	else
		shell_lvl = 1;
	tmp = ft_itoa(shell_lvl);
	shell_lvl_cmd = ft_strjoin("SHLVL=", tmp);
	free(tmp);
	ft_export((char *[]){ "export", shell_lvl_cmd, NULL }, env);
	free(shell_lvl_cmd);
}

int	main(int argc, char **argv, char **envp)
{
	int			i;
	char		*path;
	char		*join;
	t_data		*data;
	t_token		*save;
	t_token		*lexed;
	char		*in_put;
	char		**global_env;
	int			last_exit_status;

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
				// if (!data->ast)
				// 	free_parsing(data->ast, lexed);
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
