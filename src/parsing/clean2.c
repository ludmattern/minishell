/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 11:07:19 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/16 15:31:33 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

void	free_io_node(t_io_node *io_node)
{
	t_io_node	*current;
	t_io_node	*next;
	char		**temp;

	current = io_node;
	while (current != NULL)
	{
		next = current->next;
		free(current->value);
		if (current->expanded_value != NULL)
		{
			temp = current->expanded_value;
			while (*temp != NULL)
			{
				free(*temp);
				*temp = NULL;
				temp++;
			}
			free(current->expanded_value);
		}
		free(current);
		current = next;
	}
}

void	free_tree(t_node *node)
{
	if (node == NULL)
		return ;
	free_tree(node->left);
	free_tree(node->right);
	free_io_node(node->io_list);
	free(node->args);
	node->args = NULL;
	free(node->command_path);
	node->command_path = NULL;
	ft_free_double_array(node->expanded_args);
	free(node);
}

void	free_forked_data(t_data *data)
{
	if (data == NULL)
		return ;
	ft_clear_memory(data->g_data);
	free_tree(data->ast);
}