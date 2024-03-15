/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/13 15:25:18 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
* Free the io_node list.
*/
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

/*
* Free the tree.
*/
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
* Free the data structure.
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
* Main function of the shell. It will loop until the shell is closed.
*/
int	main(int argc, char **argv, char **envp)
{
	t_data	*data;
	int		last_exit_status;

	data = NULL;
	parsing(&data, argc, argv, envp);
	last_exit_status = run_execution(data);
	free_data_structure(&data);
	return (last_exit_status);
}
