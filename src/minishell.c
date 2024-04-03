/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/03 16:01:55 by lmattern         ###   ########.fr       */
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

void	initialize_shell_variables(char ***env)
{
	t_init_vars s;

	if (getcwd(s.cwd, sizeof(s.cwd)) != NULL)
	{
		s.pwd_cmd = ft_strjoin("PWD=", s.cwd);
		ft_export((char *[]){"export", s.pwd_cmd, NULL}, env);
		free(s.pwd_cmd);
	}
	ft_export((char *[]){"export", "OLDPWD", NULL}, env);
	s.shell_lvl_str = getenv("SHLVL");
	if (s.shell_lvl_str)
		s.shell_lvl = ft_atoi(s.shell_lvl_str) + 1;
	else
		s.shell_lvl = 1;
	s.tmp = ft_itoa(s.shell_lvl);
	s.shell_lvl_cmd = ft_strjoin("SHLVL=", s.tmp);
	free(s.tmp);
	ft_export((char *[]){"export", s.shell_lvl_cmd, NULL}, env);
	free(s.shell_lvl_cmd);
}

int	main(int argc, char **argv, char **envp)
{
	char		*path;
	char		*join;
	t_data		*data;
	t_token		*save;
	t_token		*lexed;
	char		*in_put;
	char		**global_env;
	char		**local_env;
	int			last_exit_status;

	(void)argv;
	(void)argc;
	//print_start();
	last_exit_status = EXIT_SUCCESS;
	global_env = duplicate_envp(envp);
	local_env = NULL;
	initialize_shell_variables(&global_env);
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
	ft_free_double_array(global_env);
	return (0);
}
