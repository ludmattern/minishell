/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   applying_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/02 11:49:09 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Redirects the input to the given file.
*/
int	redirect_input(const char *filename)
{
	int	fd;
	int	status;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (command_redirection_failure(filename, EXIT_GENERAL_ERROR));
	status = dup2(fd, STDIN_FILENO);
	close(fd);
	if (status < 0)
	{
		perror("minishell: dup2 error");
		close_standard_fds();
		return (EXIT_DUP2_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/*
Redirects the output to the given file.
*/
int	redirect_output(const char *filename)
{
	int	fd;
	int	status;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (command_redirection_failure(filename, EXIT_GENERAL_ERROR));
	status = dup2(fd, STDOUT_FILENO);
	close(fd);
	if (status < 0)
	{
		perror("minishell: dup2 error");
		close_standard_fds();
		return (EXIT_DUP2_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/*
Redirects the output to the given file, appending to it.
*/
int	redirect_append(const char *filename)
{
	int	fd;
	int	status;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (command_redirection_failure(filename, EXIT_GENERAL_ERROR));
	status = dup2(fd, STDOUT_FILENO);
	close(fd);
	if (status < 0)
	{
		perror("minishell: dup2 error");
		close_standard_fds();
		return (EXIT_DUP2_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/*
Redirects the input to a heredoc.
*/
int	redirect_heredoc(t_data *data, const char *delimiter)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) < 0)
		return (command_redirection_failure("pipe", EXIT_PIPE_FAILURE));
	pid = fork();
	if (pid < 0)
	{
		close_pipe_fds(pipefd);
		close_standard_fds();
		return (command_redirection_failure("fork", EXIT_FORK_FAILURE));
	}
	else if (pid == 0)
		return (heredoc_child_process(data, pipefd, delimiter));
	else
		return (heredoc_parent_process(pid, pipefd));
}

/*
Applies the redirections in the io_list to the current process.
*/
int	apply_command_redirections(t_data *data, t_io_node *io_list)
{
	t_io_node	*current;
	int			status;

	status = EXIT_SUCCESS;
	current = io_list;
	while (current != NULL)
	{
		if (current->type == IO_IN)
			status = redirect_input(current->expanded_value[0]);
		else if (current->type == IO_OUT)
			status = redirect_output(current->expanded_value[0]);
		else if (current->type == IO_APPEND)
			status = redirect_append(current->expanded_value[0]);
		else if (current->type == IO_HEREDOC)
			status = redirect_heredoc(data, current->value);
		if (status != EXIT_SUCCESS)
			return (status);
		current = current->next;
	}
	return (EXIT_SUCCESS);
}
