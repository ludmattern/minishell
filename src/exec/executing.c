/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/13 14:39:56 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Handles the node by calling the appropriate function based on its type.
*/
int	handling_node(t_data *data, t_node *node)
{
	if (node == NULL)
		return (EXIT_SUCCESS);
	if (node->type == N_CMD)
		return (handling_command(data, node));
	else if (node->type == N_PIPE)
		return (handling_pipeline(data, node));
	else if (node->type == N_AND)
		return (handling_and(data, node));
	else
		return (handling_or(data, node));
}

/*
Executes the AST by handling each node.
*/
int	run_execution(t_data *data)
{
	return (handling_node(data, data->ast));
}
