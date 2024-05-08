/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   applying_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/08 19:28:21 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"
#include "../../inc/parse.h"

int	redirect_heredoc_pipe(const char *heredoc_input, bool piped)
{
	int	pipefd[2];
	int	stdin_copy;

	if (pipe(pipefd) == -1)
		return (perror("pipe error"), EXIT_PIPE_FAILURE);
	write(pipefd[1], heredoc_input, ft_strlen(heredoc_input));
	if (!piped)
	{
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
			return (heredoc_dup2_creation_failure(pipefd));
	}
	else
	{
		stdin_copy = dup(STDIN_FILENO);
		if (stdin_copy == -1)
			return (heredoc_dup2_creation_failure(pipefd));
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
			return (close(stdin_copy), heredoc_dup2_creation_failure(pipefd));
	}
	close_pipe_fds(pipefd);
	return (EXIT_SUCCESS);
}

/*
Redirects the input to the given file.
*/
int	redirect_input(const char *filename, bool is_empty)
{
	int	fd;
	int	status;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (command_redirection_failure(filename, EXIT_GENERAL_ERROR));
	if (is_empty)
		return (close(fd), EXIT_SUCCESS);
	status = dup2(fd, STDIN_FILENO);
	close(fd);
	if (status < 0)
		return (perror(MS"dup2 error"), close_std_fds(), EXIT_DUP2_FAILURE);
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
		perror(MS"dup2 error");
		close_std_fds();
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
		perror(MS"dup2 error");
		close_std_fds();
		return (EXIT_DUP2_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/*
Applies the redirections in the io_list to the current process.
*/
int	apply_command_redirections(t_io_node *io_list, bool piped, bool is_empty)
{
	t_io_node	*current;
	int			status;

	status = EXIT_SUCCESS;
	current = io_list;
	while (current != NULL)
	{
		if (!handle_ambiguous(current))
			return (EXIT_GENERAL_ERROR);
		if (current->type == IO_IN)
			status = redirect_input(current->expanded_value[0], is_empty);
		else if (current->type == IO_OUT)
			status = redirect_output(current->expanded_value[0]);
		else if (current->type == IO_APPEND)
			status = redirect_append(current->expanded_value[0]);
		else if (current->type == IO_HEREDOC && !is_empty)
			status = redirect_heredoc_pipe(current->expanded_value[0], piped);
		if (status != EXIT_SUCCESS)
			return (status);
		current = current->next;
	}
	return (EXIT_SUCCESS);
}
