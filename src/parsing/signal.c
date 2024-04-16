/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 17:36:12 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/16 17:05:04 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"
#include "../inc/exec.h"

void handle_sigint_herdoc(int sig) 
{
	(void)sig;
	g_heredoc_sigint = 2;
	write(1, "\n", 1);
	rl_done = 1;
	rl_redisplay();
	
}

void handle_sigint(int sig) 
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

void	proc_handle_sigint(int sig)
{
	printf("\n");
	(void)sig;
	exit(2);
}

void	proc_handle_sigquit(int sig)
{
	(void)sig;
	exit(3);
}

void	signals_init(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
