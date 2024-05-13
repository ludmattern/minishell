/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/13 10:19:27 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/bonus/parse_bonus.h"
#include "../inc/bonus/exec_bonus.h"

int	g_heredoc_sigint = 0;

char	*init_bash(char *host)
{
	static char	buffer[1024];
	char		*tmp;
	char		*tmp2;
	char		*usr;

	buffer[0] = 0;
	usr = getenv("USER");
	tmp = getenv("SESSION_MANAGER");
	if (!usr || !usr[0] || !tmp || !tmp[0])
		return (ft_sprintf(buffer, "minishell:"), buffer);
	if (ft_strlen(tmp) < 6)
		return (buffer);
	tmp2 = ft_strnstr(tmp, "local/", 6);
	if (tmp2)
	{
		if (ft_strlen(tmp2 + 6) < 6)
			return (buffer);
		host = ft_strndup(tmp2 + 6, 6);
		if (!host)
			return (NULL);
	}
	return (ft_sprintf(buffer, "%s@%s:", usr, host), free(host), buffer);
}

void	handle_user_input(t_g_data *g_data)
{
	update_data(g_data);
	if (g_data->in_put[0] != -1 || g_data->in_put[1] != '\0')
	{
		launch_lexing(g_data);
		if (g_data->exit_fail == -1)
		{
			free_lexed(g_data->lexed);
			fail_exit_shell(g_data);
		}
		launch_expand(g_data);
		if (g_heredoc_sigint == 2)
			free_and_init(g_data);
		else
		{
			launch_parsing(g_data);
			signals_init_exec();
			launch_execution(g_data);
			signals_init();
		}
	}
	else
		free(g_data->data);
}

void	main_clean_exit(t_g_data *g_data)
{
	free_mini_env(g_data->mini_env);
	free(g_data);
	close_std_fds();
	exit(EXIT_GENERAL_ERROR);
}

void	init_minishell(t_g_data **g_data, char **env, char **av, int ac)
{
	(void)av;
	if (ac != 1)
	{
		ft_putstr_fd("minishell: no arguments needed\n", STDERR_FILENO);
		close_std_fds();
		exit(EXIT_GENERAL_ERROR);
	}
	signals_init();
	*g_data = ft_calloc(1, sizeof(t_g_data));
	if (!*g_data)
	{
		close_std_fds();
		exit(EXIT_GENERAL_ERROR);
	}
	(*g_data)->mini_env = create_mini_env(env, g_data);
	if (!initialize_shell_variables(&(*g_data)->mini_env))
		main_clean_exit(*g_data);
	(*g_data)->pre_input = init_bash(NULL);
	if (!(*g_data)->pre_input)
		main_clean_exit(*g_data);
	(*g_data)->t = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_g_data	*g_data;

	init_minishell(&g_data, envp, argv, argc);
	while (1)
	{
		update_input(g_data, g_data->pre_input);
		if (g_data->in_put[0])
		{
			if (!syntax_error(g_data->in_put, &g_data->last_exit_status))
				handle_user_input(g_data);
			update_history(g_data);
		}
	}
	ft_clear_memory(g_data);
	return (0);
}
