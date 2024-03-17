/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_pipeline_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/17 11:00:17 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Closes the standard file descriptors.
*/
void	close_standard_fds(void)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
}

/*
Prints an error message and exit the child process if dup2 fails.
*/
void	dup2_creation_failure(t_data *data, int pipefd[2])
{
	(void)data;
	perror("minishell: dup2 error");
	close_pipe_fds(pipefd);
	//free_data_structure(&data);
	close_standard_fds();
	exit(EXIT_DUP2_FAILURE);
}

/*
Waits for the child processes to finish.
*/
void	wait_for_pipeline_children(pid_t pid1, pid_t pid2)
{
	int	status;

	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
}

/*
Closes the pipe file descriptors.
*/
void	close_pipe_fds(int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
}

/*
Creates a pipe and checks for errors in the process.
*/
int	create_pipe(int pipefd[2])
{
	if (pipe(pipefd) < 0)
	{
		perror("minishell: pipe error");
		return (EXIT_PIPE_FAILURE);
	}
	else
		return (EXIT_SUCCESS);
}
