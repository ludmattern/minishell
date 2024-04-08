/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/07 17:47:52 by fprevot          ###   ########.fr       */
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
		if (mini_env->value)
		{
			temp = ft_strjoin(mini_env->name, "=");
			(*env)[i] = ft_strjoin(temp, mini_env->value);
			free(temp);
		}
		else
			(*env)[i] = ft_strdup(mini_env->name);
		mini_env = mini_env->next;
		i++;
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
	if (node->is_add_local == true)
		return (true);
	else if (ft_strncmp(str, "/", ft_strlen(str) == 0))
		return (false);
	else if (ft_strncmp(str, "cd", 2) == 0 || ft_strncmp(str, "export", 6) == 0
		|| ft_strncmp(str, "unset", 5) == 0 || ft_strncmp(str, "exit", 4) == 0)
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
	else if (ft_strncmp(str, "cd", 2) == 0)
		status = ft_cd(node->expanded_args, &data->mini_env);
	else if (ft_strncmp(str, "export", 6) == 0)
		status = ft_export(node->expanded_args, data);
	else if (ft_strncmp(str, "exit", 4) == 0)
		status = ft_exit(node->expanded_args, &data);
	else
		status = ft_unset_vars(node->expanded_args, &data->mini_env);
	return (status);
}

int	launch_non_forked_builtins(t_data *data, t_node *node, bool piped)
{
	int	status;

	status = apply_command_redirections(data, node->io_list);
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

static void	proc_handle_sigint(int sig)
{
	(void)sig;
	exit(2);
}

static void	proc_handle_sigquit(int sig)
{
	(void)sig;
	exit(3);
}
/*static void	sigign(int sig)
{
	(void)sig;
	signal(SIGINT, SIG_IGN);
	printf("no heredoc pls\n");
}*/
int	handle_command_child(t_data *data, t_node *node)
{
	int	status;

	status = apply_command_redirections(data, node->io_list);
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

	if (node->expanded_args[0] == NULL)
		return (EXIT_SUCCESS);
	if (update_last_arg(data, node))
		return (EXIT_FAILURE);
	if (is_non_forked_builtins(node))
		return (launch_non_forked_builtins(data, node, piped));
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, proc_handle_sigint);
		signal(SIGQUIT, proc_handle_sigquit);
		return (handle_command_child(data, node));
	}
	else if (pid > 0)
	{
		signals_init();
		return (wait_for_child(pid, data));
	}
	else
		return (fork_creation_failure("fork"));
}
