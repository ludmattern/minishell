/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 17:11:48 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/15 11:52:43 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

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
	char	**temp;

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

/* 
Frees the data structure and its content.
*/
void	free_data(t_data *data)
{
	char	**temp;

	if (data == NULL)
		return ;
	if (data->env != NULL)
	{
		temp = data->env;
		while (*temp != NULL)
		{
			free(*temp);
			temp++;
		}
		free(data->env);
	}
	free_tree(data->ast);
}

/*
Frees the data structure.
*/
void	free_data_structure(t_data **data)
{
	free_data(*data);
	free(*data);
}