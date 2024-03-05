/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   applying_redirections_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/05 13:43:44 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Handles the child process of a pipeline.
*/
void	heredoc_child_process(int pipefd[2], const char *delimiter)
{
	close(pipefd[0]);
	read_heredoc_and_write_to_pipe(delimiter, pipefd[1]);
	close(pipefd[1]);
	exit(EXIT_SUCCESS);
}

/*
Close the pipe file descriptors and wait for the child process to finish.
*/
void	heredoc_parent_process(int pipefd[2])
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	wait(NULL);
}
