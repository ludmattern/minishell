/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/18 15:05:22 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"
#include "../../inc/parse.h"

size_t	ft_env_size(t_env *lst)
{
	int	i;

	if (!lst)
		return (0);
	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

void	transform_env_to_array(t_env *mini_env, char ***env)
{
	char	*temp;
	size_t	env_size;
	size_t	i;

	env_size = ft_env_size(mini_env);
	if (env_size == 0)
	{
		*env = NULL;
		return ;
	}
	*env = ft_calloc(env_size + 1, sizeof(char *));
	i = 0;
	while (mini_env)
	{
		if (mini_env->is_local == false)
		{
			if (mini_env->value)
			{
				temp = ft_strjoin(mini_env->name, "=");
				(*env)[i] = ft_strjoin(temp, mini_env->value);
				free(temp);
			}
			else
				(*env)[i] = ft_strdup(mini_env->name);
			i++;
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

bool	is_non_forked_builtins(t_node *node)
{
	const char	*str;

	str = (const char *)node->expanded_args[0];
	if (node->is_empty)
		return (true);
	if (node->is_add_local == true)
		return (true);
	else if (ft_strncmp(str, "/", ft_strlen(str) == 0))
		return (false);
	else if (ft_strncmp(str, "cd", 3) == 0 || ft_strncmp(str, "export", 7) == 0
		|| ft_strncmp(str, "unset", 6) == 0 || ft_strncmp(str, "exit", 5) == 0)
		return (true);
	else
		return (false);
}

int	execute_non_forked_builtins(t_data *data, t_node *node)
{
	int			status;
	const char	*str;

	str = (const char *)node->expanded_args[0];
	if (node->is_add_local == true)
		status = ft_add_local(node->expanded_args[0], &data->mini_env);
	else if (ft_strncmp(str, "cd", 3) == 0)
		status = ft_cd(node->expanded_args, &data->mini_env);
	else if (ft_strncmp(str, "export", 7) == 0)
		status = ft_export(node->expanded_args, data);
	else if (ft_strncmp(str, "exit", 5) == 0)
		status = ft_exit(node->expanded_args, &data);
	else
		status = ft_unset_vars(node->expanded_args, &data->mini_env);
	return (status);
}

int	launch_non_forked_builtins(t_data *data, t_node *node, bool piped)
{
	int	status;

	status = apply_command_redirections(node->io_list, piped, node->is_empty);
	if (node->is_empty)
		return (status);
	if (status == EXIT_SUCCESS)
		status = execute_non_forked_builtins(data, node);
	if (piped)
		restore_original_fds(data);
	return (status);
}

int	update_last_arg(t_data *data, t_node *node)
{
	char	*last_arg;
	size_t	nb_args;

	nb_args = ft_double_array_len(node->expanded_args);
	last_arg = ft_strdup(node->expanded_args[nb_args - 1]);
	if (!last_arg)
		return (EXIT_GENERAL_ERROR);
	if (add_or_update_env(&data->mini_env, "_", last_arg, true))
		return (ft_free(last_arg), EXIT_GENERAL_ERROR);
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
	signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		return (handle_command_child(data, node, piped));
	else if (pid > 0)
	{
		exit_status = wait_for_child(pid, data);
		signals_init();
		return (exit_status);

	}
	else
		return (fork_creation_failure("fork"));
}
