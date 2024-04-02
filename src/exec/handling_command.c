/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/02 17:14:02 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

void	command_exec_failure(t_data *data, const char *context, int exit_code)
		__attribute__((noreturn));
void	execute_command(t_data *data, t_node *node) __attribute__((noreturn));

/*
Prints an error message and exit the child process.
*/
void	command_exec_failure(t_data *data, const char *context, int error_code)
{
	int	status;

	status = EXIT_COMMAND_NOT_FOUND;
	if (error_code == EXIT_COMMAND_NOT_FOUND)
	{
		if (ft_strchr(context, '/'))
			ft_eprintf("minishell: %s: No such file or directory\n", context);
		else
			ft_eprintf("minishell: %s: command not found\n", context);
	}
	else if (error_code == EXIT_PERMISSION_DENIED)
	{
		if (ft_strchr(context, '/'))
		{
			status = EXIT_INVALID_COMMAND;
			ft_eprintf("minishell: %s: Permission denied\n", context);
		}
		else
			ft_eprintf("minishell: %s: command not found\n", context);
	}
	else if (error_code == EXIT_IS_A_DIRECTORY)
	{
		if (ft_strchr(context, '/'))
		{
			status = EXIT_INVALID_COMMAND;
			ft_eprintf("minishell: %s: Is a directory\n", context);
		}
		else
			ft_eprintf("minishell: %s: command not found\n", context);
	}
	else
		ft_eprintf("minishell: %s: %s\n", context, strerror(errno));
	free_forked_data_structure(&data);
	exit(status);
}

void	execute_command(t_data *data, t_node *node)
{
	struct stat	statbuf;

	checking_forked_builtins(data, node);
	if (stat(node->command_path, &statbuf) == -1)
		command_exec_failure(data, node->expanded_args[0],
			EXIT_COMMAND_NOT_FOUND);
	if (S_ISDIR(statbuf.st_mode))
		command_exec_failure(data, node->expanded_args[0], EXIT_IS_A_DIRECTORY);
	if (access(node->command_path, X_OK) == -1)
		command_exec_failure(data, node->expanded_args[0],
			EXIT_PERMISSION_DENIED);
	execve(node->command_path, node->expanded_args, data->env);
	command_exec_failure(data, node->expanded_args[0], EXIT_GENERAL_ERROR);
}

int	fork_creation_failure(const char *message)
{
	ft_eprintf("minishell: %s: %s\n", message, strerror(errno));
	return (EXIT_FORK_FAILURE);
}

bool	is_non_forked_builtins(t_node *node)
{
	const char	*str;

	str = (const char *)node->expanded_args[0];
	if (ft_strncmp(str, "/", ft_strlen(str) == 0))
		return (false);
	else if (ft_strncmp(str, "cd", 2) == 0 || ft_strncmp(str, "export", 6) == 0
		|| ft_strncmp(str, "unset", 5) == 0)
		return (true);
	else
		return (false);
}

int	execute_non_forked_builtins(t_data *data, t_node *node)
{
	int			status;
	const char	*str;

	str = (const char *)node->expanded_args[0];
	if (ft_strncmp(str, "cd", 2) == 0)
		status = ft_cd(node->expanded_args, data->env);
	else if (ft_strncmp(str, "export", 6) == 0)
		status = ft_export(node->expanded_args, &data->env);
	else
		status = ft_unset(node->expanded_args[0], &data->env);
	return (status);
}

void	restore_original_fds(t_data *data)
{
	dup2(data->stdin, STDIN_FILENO);
	dup2(data->stdout, STDOUT_FILENO);
}

/*
Executes the command in a child process and waits for it to finish.
*/
int	handling_command(t_data *data, t_node *node, bool piped)
{
	pid_t	pid;
	int		status;

	if (node->expanded_args[0] == NULL)
		return (EXIT_SUCCESS);
	if (is_non_forked_builtins(node))
	{
		status = apply_command_redirections(data, node->io_list);
		if (status == EXIT_SUCCESS)
			status = execute_non_forked_builtins(data, node);
		if (piped)
			restore_original_fds(data);
		return (status);
	}
	pid = fork();
	if (pid == 0)
	{
		status = apply_command_redirections(data, node->io_list);
		if (status == EXIT_SUCCESS)
			execute_command(data, node);
		else
		{
			free_forked_data_structure(&data);
			exit(status);
		}
	}
	else if (pid > 0)
		return (wait_for_child(pid, data));
	else
		return (fork_creation_failure("fork"));
}
