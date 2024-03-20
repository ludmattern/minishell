/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 17:11:48 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/20 16:19:59 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

/*
Frees the io_node and its content.
*/
void	free_io_node(t_io_node *io_node)
{
	t_io_node	*current;
	t_io_node	*next;
	char    **temp;

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

/*
Frees the tree and its content.
*/
void free_tree(t_node *node) 
{
	char **temp;

	if (node == NULL)
		return;
	free_tree(node->left);
	free_tree(node->right);
	free_io_node(node->io_list);
	free(node->args);  
	node->args = NULL;
	free(node->command_path); 
	node->command_path = NULL;

	if (node->expanded_args != NULL)
	{
		temp = node->expanded_args;
		while (*temp != NULL)
		{
			free(*temp); 
			*temp = NULL;
			temp++;
		}
		free(node->expanded_args);  
		node->expanded_args = NULL;
	}
	free(node);
}

/* 
Frees the data structure and its content.
*/
void	free_forked_data(t_data *data)
{
	int i;

	i = 0;
	if (data == NULL)
		return ;
	while (data->env[i] != NULL)
	{
		free(data->env[i]);
		i++;
	}
	free(data->env);
	free_tree(data->ast);
}

/* 
Frees the data structure and its content.
*/
void	free_data(t_data *data)
{
	if (data == NULL)
		return ;
	free_tree(data->ast);
}

/*
Frees the data structure.
*/
void	free_forked_data_structure(t_data **data)
{
	free_forked_data(*data);
	free(*data);
}

/*
Frees the data structure.
*/
void	free_data_structure(t_data **data)
{
	free_data(*data);
	free(*data);
}