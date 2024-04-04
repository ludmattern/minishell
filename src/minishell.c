/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/04 13:04:33 by lmattern         ###   ########.fr       */
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

t_global_data	initialize_environnement(char **envp)
{
	t_global_data	g_data;

	memset(&g_data, 0, sizeof(t_global_data));
	g_data.last_exit_status = EXIT_SUCCESS;
	g_data.global_env = duplicate_envp(envp);
	g_data.local_env = NULL;
	initialize_shell_variables(&g_data.global_env);
	signal(SIGINT, sigint_handler);
	return (g_data);
}

void	update_data(t_global_data *g_data)
{
	g_data->data = malloc(sizeof(t_data));
	//if (!data)
	//	return (printf("MALLOC ERROR"));
	ft_bzero(g_data->data, sizeof(t_data));
	g_data->data->env = g_data->global_env;
	g_data->data->l_env = g_data->local_env;
	g_data->data->last_exit_status = g_data->last_exit_status;
}

void launch_lexing(t_global_data *g_data)
{
	g_data->lexed = lex_me(g_data->in_put);
	if (g_data->lexed->error == -1)
		lex_mallox_error(g_data->lexed);
	g_data->save = g_data->lexed;
	g_data->lexed->g_data = g_data;
}

void launch_parsing(t_global_data *g_data)
{
	g_data->data->ast = build_ast(&g_data->lexed, g_data->data->last_exit_status);
	// if (!data->ast)
	// 	free_parsing(data->ast, lexed);
	free_lexed(g_data->save);
}

void	launch_execution(t_global_data *g_data)
{
	g_data->last_exit_status = run_execution(g_data->data);
	g_data->global_env = g_data->data->env;
	g_data->local_env = g_data->data->l_env;
	free_data_structure(&g_data->data);
}

void	update_history(t_global_data *g_data)
{
	add_history(g_data->in_put);
	free(g_data->in_put);
	g_data->in_put = NULL;
}

void	ft_clear_memory(t_global_data *g_data)
{
	ft_free_double_array(g_data->global_env);
	ft_free_double_array(g_data->local_env);
}

void	update_input(t_global_data *g_data)
{
	g_data->path = getcwd(NULL, 0);
	g_data->join = ft_strjoin(g_data->path, " $> ");
	g_data->in_put = readline(g_data->join);
	free(g_data->join);
	free(g_data->path);
	if (!g_data->in_put) 
	{
		write(STDOUT_FILENO, "\n", 1);  
		ft_clear_memory(g_data);
		exit(EXIT_GENERAL_ERROR);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_global_data	g_data;

	(void)argv;
	(void)argc;
	//print_start();
	g_data = initialize_environnement(envp);
	while (1)
	{
		update_input(&g_data);
		if (g_data.in_put[0])
		{
			if (syntax_error(g_data.in_put, &g_data.last_exit_status))
				continue ;
			else
			{
				update_data(&g_data);
				launch_lexing(&g_data);
				launch_parsing(&g_data);
				launch_execution(&g_data);
			}
			update_history(&g_data);
		}
	}
	ft_clear_memory(&g_data);
	return (0);
}
