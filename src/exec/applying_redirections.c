/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   applying_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/06 18:02:31 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

void	command_redirection_failure(const char *message, int exit_code)
{
	ft_eprintf("minishell: %s: %s\n", message, strerror(errno));
	exit(exit_code);
}

/*
Redirects the input to the given file.
*/
void	redirect_input(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		if (errno == EACCES)
			command_redirection_failure("open", EXIT_PERMISSION_DENIED);
		else
			command_redirection_failure("open", EXIT_GENERAL_ERROR);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

/*
Redirects the output to the given file.
*/
void	redirect_output(const char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		if (errno == EACCES)
			command_redirection_failure("open", EXIT_PERMISSION_DENIED);
		else
			command_redirection_failure("open", EXIT_GENERAL_ERROR);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

/*
Redirects the output to the given file, appending to it.
*/
void	redirect_append(const char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		if (errno == EACCES)
			command_redirection_failure("open", EXIT_PERMISSION_DENIED);
		else
			command_redirection_failure("open", EXIT_GENERAL_ERROR);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

/*
Redirects the input to a heredoc.
*/
void	redirect_heredoc(const char *delimiter)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) < 0)
	{
		command_redirection_failure("pipe", EXIT_PIPE_FAILURE);
	}
	pid = fork();
	if (pid < 0)
		command_redirection_failure("fork", EXIT_FORK_FAILURE);
	else if (pid == 0)
		heredoc_child_process(pipefd, delimiter);
	else
		heredoc_parent_process(pipefd);
}

/*
Applies the redirections in the io_list to the current process.
*/
void	apply_command_redirections(t_io_node *io_list)
{
	t_io_node	*current;

	current = io_list;
	while (current != NULL)
	{
		if (current->type == IO_IN)
			redirect_input(current->expanded_value);
		else if (current->type == IO_OUT)
			redirect_output(current->expanded_value);
		else if (current->type == IO_APPEND)
			redirect_append(current->expanded_value);
		else if (current->type == IO_HEREDOC)
			redirect_heredoc(current->value);
		current = current->next;
	}
}
