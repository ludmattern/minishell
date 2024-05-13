/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/13 10:19:46 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/mandatory/parse.h"
#include "../inc/mandatory/exec.h"

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

void	handle_user_input(t_g_data *t_data)
{
	update_data(t_data);
	if (t_data->in_put[0] != -1 || t_data->in_put[1] != '\0')
	{
		launch_lexing(t_data);
		if (t_data->exit_fail == -1)
		{
			free_lexed(t_data->lexed);
			fail_exit_shell(t_data);
		}
		launch_expand(t_data);
		if (g_heredoc_sigint == 2)
			free_and_init(t_data);
		else
		{
			launch_parsing(t_data);
			signals_init_exec();
			launch_execution(t_data);
			signals_init();
		}
	}
	else
		free(t_data->data);
}

void	main_clean_exit(t_g_data *t_data)
{
	free_mini_env(t_data->mini_env);
	free(t_data);
	close_std_fds();
	exit(EXIT_GENERAL_ERROR);
}

void	init_minishell(t_g_data **t_data, char **env, char **av, int ac)
{
	(void)av;
	if (ac != 1)
	{
		ft_putstr_fd("minishell: no arguments needed\n", STDERR_FILENO);
		close_std_fds();
		exit(EXIT_GENERAL_ERROR);
	}
	signals_init();
	*t_data = ft_calloc(1, sizeof(t_g_data));
	if (!*t_data)
	{
		close_std_fds();
		exit(EXIT_GENERAL_ERROR);
	}
	(*t_data)->mini_env = create_mini_env(env, t_data);
	if (!initialize_shell_variables(&(*t_data)->mini_env))
		main_clean_exit(*t_data);
	(*t_data)->pre_input = init_bash(NULL);
	if (!(*t_data)->pre_input)
		main_clean_exit(*t_data);
	(*t_data)->t = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_g_data	*t_data;

	init_minishell(&t_data, envp, argv, argc);
	while (1)
	{
		update_input(t_data, t_data->pre_input);
		if (t_data->in_put[0])
		{
			if (!syntax_error(t_data->in_put, &t_data->last_exit_status))
				handle_user_input(t_data);
			update_history(t_data);
		}
	}
	ft_clear_memory(t_data);
	return (0);
}
