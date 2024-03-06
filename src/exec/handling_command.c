/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/06 18:23:04 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

void	command_exec_failure(const char *context, int exit_code) \
__attribute__((noreturn));
void	execute_command(t_data *data, t_node *node) __attribute__((noreturn));

/*
Prints an error message and exit the child process.
*/
void	command_exec_failure(const char *context, int error_code)
{
	if (error_code == EXIT_COMMAND_NOT_FOUND)
	{
		if (ft_strchr(context, '/'))
			ft_eprintf("minishell: %s: no such file or directory\n", context);
		else
			ft_eprintf("minishell: %s: command not found\n", context);
	}
	else if (error_code == EXIT_PERMISSION_DENIED)
		ft_eprintf("minishell: %s: Permission denied\n", context);
	else if (error_code == EXIT_IS_A_DIRECTORY)
		ft_eprintf("minishell: %s: Is a directory\n", context);
	else
		ft_eprintf("minishell: %s: %s\n", context, strerror(errno));
	exit(EXIT_COMMAND_NOT_FOUND);
}

void	execute_command(t_data *data, t_node *node)
{
	struct stat	statbuf;

	if (stat(node->command_path, &statbuf) == -1)
		command_exec_failure(node->expanded_args[0], EXIT_COMMAND_NOT_FOUND);
	if (S_ISDIR(statbuf.st_mode))
		command_exec_failure(node->expanded_args[0], EXIT_IS_A_DIRECTORY);
	if (access(node->command_path, X_OK) == -1)
		command_exec_failure(node->expanded_args[0], EXIT_PERMISSION_DENIED);
	execve(node->command_path, node->expanded_args, data->env);
	command_exec_failure(node->expanded_args[0], EXIT_GENERAL_ERROR);
}

void	fork_creation_failure(const char *message, int error_code)
{
	ft_eprintf("minishell: %s: %s\n", message, strerror(errno));
	exit(error_code);
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
		fork_creation_failure("fork", EXIT_FORK_FAILURE);
}
