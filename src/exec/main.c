/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/08 17:45:29 by lmattern         ###   ########.fr       */
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
	if (data == NULL)
		return ;
	free_tree(data->ast);
}

/*
* Main function of the shell. It will loop until the shell is closed.
*/
int	main(int argc, char **argv, char **envp)
{
	t_data	*data;
	int		last_exit_status;

	(void)argc;
	(void)argv;
	data = malloc(sizeof(t_data));
	if (data == NULL)
		return (EXIT_GENERAL_ERROR);
	ft_bzero(data, sizeof(t_data));
	data->env = envp;
	data->ast = create_ast();
	data->last_exit_status = EXIT_SUCCESS;
	last_exit_status = run_execution(data);
	free_data(data);
	free(data);
	return (last_exit_status);
}
