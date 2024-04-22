/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:38:04 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/22 11:19:18 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"
#include "../inc/exec.h"

void	proc_handle_sigint(int sig)
{
	printf("\n");
	(void)sig;
	exit(2);
}

void	proc_handle_sigquit(int sig)
{
	(void)sig;
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

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_sigint_herdoc;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}
