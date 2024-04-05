/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   applying_redirections_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/05 18:26:40 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"
#include "../../inc/parse.h"

/*
print an error message and return the exit code.
*/
int	command_redirection_failure(const char *context, int exit_code)
{
	ft_eprintf("minishell: %s: %s\n", context, strerror(errno));
	return (exit_code);
}

/*
Closes the standard file descriptors after printing and error msg and return 
the exit code.
*/
int	heredoc_redirection_failure(const char *context, int exit_code)
{
	ft_eprintf("minishell: %s: %s\n", context, strerror(errno));
	close_standard_fds();
	return (exit_code);
}

/*
Handles the child process of a pipeline.
*/
int	heredoc_child_process(t_data *data, int pipefd[2], const char *delimiter)
{
	int	status;
	
	signal(SIGINT, handle_sigint_heredoc);
	close(pipefd[0]);
	status = read_heredoc_and_write_to_pipe(delimiter, pipefd[1]);
	close(pipefd[1]);
	close_standard_fds();
	free_forked_data_structure(&data);
	
	exit(status);
}

/*
Close the pipe file descriptors and wait for the child process to finish.
*/
int	heredoc_parent_process(pid_t pid, int pipefd[2])
{
	int	status;

	signal(SIGINT, SIG_IGN);
	close(pipefd[1]);
	status = dup2(pipefd[0], STDIN_FILENO);
	if (status < 0)
	{
		waitpid(pid, &status, 0);
		close_pipe_fds(pipefd);
		return (heredoc_redirection_failure("dup2", EXIT_DUP2_FAILURE));
	}
	close(pipefd[0]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS)
		return (heredoc_redirection_failure("heredoc", EXIT_GENERAL_ERROR));
	signals_init();
	return (status);
}
