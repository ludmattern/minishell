/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_pipeline_utils_bonus.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/13 10:26:46 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/bonus/exec_bonus.h"
#include "../../inc/bonus/parse_bonus.h"

/*
Closes the standard file descriptors.
*/
void	close_std_fds(void)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

/*
Prints an error message and exit the child process if dup2 fails.
*/
void	dup2_creation_failure(t_data *data, int pipefd[2])
{
	perror(MS"dup2 error");
	close_pipe_fds(pipefd);
	free_forked_data_structure(&data);
	close_std_fds();
	exit(EXIT_DUP2_FAILURE);
}

/*
Waits for the child processes to finish.
*/
int	wait_for_pipeline_children(pid_t pid1, pid_t pid2)
{
	int	status;
	int	return_status;

	return_status = EXIT_SUCCESS;
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		return_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		return_status = 128 + WTERMSIG(status);
	else
		return_status = EXIT_FAILURE;
	return (return_status);
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
		return (perror(MS"pipe error"), EXIT_PIPE_FAILURE);
	else
		return (EXIT_SUCCESS);
}
