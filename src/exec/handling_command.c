/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/21 16:10:17 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"
#include "../../inc/parse.h"

void	transform_env_to_array(t_env *mini_env, char ***env)
{
	char	*temp;
	size_t	env_size;
	size_t	i;

	env_size = ft_env_size(mini_env);
	if (env_size == 0)
		return (*env = NULL, (void)0);
	*env = ft_calloc(env_size + 1, sizeof(char *));
	i = 0;
	while (mini_env)
	{
		if (mini_env->is_local == false)
		{
			if (mini_env->value)
			{
				temp = ft_strjoin(mini_env->name, "=");
				(*env)[i++] = ft_strjoin(temp, mini_env->value);
				free(temp);
			}
			else
				(*env)[i++] = ft_strdup(mini_env->name);
		}
		mini_env = mini_env->next;
	}
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
	if (data->env)
		ft_free_double_array(data->env);
	transform_env_to_array(data->mini_env, &data->env);
	execve(node->command_path, node->expanded_args, data->env);
	ft_free_double_array(data->env);
	command_exec_failure(data, node->expanded_args[0], EXIT_GENERAL_ERROR);
}

int	update_last_arg(t_data *data, t_node *node)
{
	char	*last_arg;
	char	*tmp;
	size_t	nb_args;

	nb_args = ft_double_array_len(node->expanded_args);
	last_arg = ft_strdup(node->expanded_args[nb_args - 1]);
	if (!last_arg)
		return (EXIT_GENERAL_ERROR);
	tmp = NULL;
	tmp = ft_strdup("_");
	if (!tmp)
		return (ft_free(last_arg), EXIT_GENERAL_ERROR);
	if (add_or_update_env(&data->mini_env, tmp, last_arg, true))
		return (free(tmp), ft_free(last_arg), EXIT_GENERAL_ERROR);
	return (EXIT_SUCCESS);
}

int	handle_command_child(t_data *data, t_node *node, bool piped)
{
	int	status;

	signal(SIGINT, proc_handle_sigint);
	signal(SIGQUIT, proc_handle_sigquit);
	status = apply_command_redirections(node->io_list, piped, node->is_empty);
	if (status == EXIT_SUCCESS)
		execute_command(data, node);
	else
	{
		free_forked_data_structure(&data);
		exit(status);
	}
	return (EXIT_GENERAL_ERROR);
}

/*
Executes the command in a child process and waits for it to finish.
*/
int	handling_command(t_data *data, t_node *node, bool piped)
{
	pid_t	pid;
	int		exit_status;

	if (node->expanded_args[0] == NULL)
		return (EXIT_SUCCESS);
	if (update_last_arg(data, node))
		return (EXIT_FAILURE);
	if (is_non_forked_builtins(node))
	{
		exit_status = launch_non_forked_builtins(data, node, piped);
		if (node->is_empty)
			restore_original_fds(data);
		return (exit_status);
	}
	signals_ignore();
	pid = fork();
	if (pid == 0)
		return (handle_command_child(data, node, piped));
	else if (pid > 0)
	{
		exit_status = wait_for_child(pid, data);
		return (signals_init(), exit_status);
	}
	else
		return (fork_creation_failure("fork"));
}
