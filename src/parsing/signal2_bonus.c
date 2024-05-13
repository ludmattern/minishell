/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:38:04 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/13 10:33:00 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/bonus/parse_bonus.h"
#include "../../inc/bonus/exec_bonus.h"

void	signals_init_exec(void)
{
	signal(SIGINT, proc_handle_sigint);
	signal(SIGQUIT, proc_handle_sigquit);
}

void	proc_handle_sigint(int sig)
{
	printf("\n");
	(void)sig;
}

void	proc_handle_sigquit(int sig)
{
	(void)sig;
	printf("Quit (core dumped)\n");
}

void	handle_sigint_herdoc(int sig)
{
	(void)sig;
	g_heredoc_sigint = 2;
	rl_done = 1;
	rl_redisplay();
}

void	setup_heredoc_si(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_sigint_herdoc;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}
