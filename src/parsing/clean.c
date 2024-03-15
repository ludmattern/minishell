/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 17:11:48 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/15 10:40:42 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

void	free_io_node(t_io_node *io_node)
{
	t_io_node	*current;
	t_io_node	*next;

	current = io_node;
	while (current != NULL)
	{
		next = current->next;
		free(current->value);
		free(current->expanded_value);
		free(current);
		current = next;
	}
}

void	free_tree(t_node *node)
{
	//char	**temp;

	if (node == NULL)
		return ;
	free_tree(node->left);
	free_tree(node->right);
	free_io_node(node->io_list);
	free(node->args);
	free(node->command_path);
	if (node->expanded_args != NULL)
	{
		temp = node->expanded_args;
		while (*temp != NULL)
		{
			free(*temp);
			temp++;
		}
		free(node->expanded_args);
	}
	free(node);
}

void	free_data(t_data *data)
{
	if (data == NULL)
		return ;
	free_tree(data->ast);
}

void	free_data_structure(t_data **data)
{
	free_data(*data);
	free(*data);
}