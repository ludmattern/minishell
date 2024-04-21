/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_builtins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/21 16:08:54 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"
#include "../../inc/parse.h"

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
