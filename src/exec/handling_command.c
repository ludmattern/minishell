/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/05 17:25:50 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

void	command_execution_failure(const char *error, int exit_code) __attribute__((noreturn));
void	execute_command(t_data *data, t_node *node) __attribute__((noreturn));

/*
Prints an error message and exit the child process.
*/
void	command_execution_failure(const char *error, int exit_code)
{
	perror(error);
	exit(exit_code);
}

/*
Executes the command in the child process.
*/
void	execute_command(t_data *data, t_node *node)
{
	execve(node->expanded_args[0], node->expanded_args, data->env);
	if (errno == ENOENT)
		command_execution_failure("command not found", EXIT_COMMAND_NOT_FOUND);
	else if (errno == EACCES)
		command_execution_failure("permission denied", EXIT_PERMISSION_DENIED);
	else
		command_execution_failure("execve error", EXIT_EXEC_FAILURE);
}

/*
Executes the command in a child process and waits for it to finish.
*/
int	handling_command(t_data *data, t_node *node)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		apply_command_redirections(node->io_list);
		execute_command(data, node);
	}
	else if (pid > 0)
		return (wait_for_child(pid, data));
	else
		command_execution_failure("fork error", EXIT_FORK_FAILURE);
}
