/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 15:37:39 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/06 15:41:53 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/exec.h"

void	update_input(t_g_data *g_data)
{
	g_data->path = getcwd(NULL, 0);
	g_data->join = ft_strjoin(g_data->path, " $> ");
	g_data->in_put = readline(g_data->join);
	free(g_data->join);
	free(g_data->path);
	if (!g_data->in_put) 
	{
		write(STDOUT_FILENO, "exit\n", 5);  
		ft_clear_memory(g_data);
		exit(EXIT_SUCCESS);
	}
}

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	(void)sig;
	rl_redisplay();
}

void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(1);
}

void	signals_init(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}
