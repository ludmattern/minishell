/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:19:00 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/01 17:47:34 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

void	print_io_list(t_io_node* node)
{
	while (node != NULL)
	{
		printf("IO Node: type=%d, value=%s, here_doc=%d\n", node->type, node->value, node->here_doc);
		node = node->next;
	}
}

void print_ast(t_node* node, int depth)
{
	if (node == NULL)
		return;

	// Adjust the indentation based on the depth
	for (int i = 0; i < depth; i++)
		printf("  ");

	// Print the node type and additional details if applicable
	switch (node->type)
	{
		case N_PIPE:
			printf("PIPE\n");
			break;
		case N_AND:
			printf("AND\n");
			break;
		case N_OR:
			printf("OR\n");
			break;
		case N_CMD:
			printf("CMD: %s\n", node->args); // Consider including args details if applicable
			break;
		default:
			printf("UNKNOWN\n");
			break;
	}

	// Recursively print the left and right children, with adjusted depth
	// Indicate the child type (left or right) for clarity
	if (node->left != NULL)
	{
		for (int i = 0; i <= depth; i++) printf("  ");
		printf("-> Left:\n");
		print_ast(node->left, depth + 1);
	}
	if (node->right != NULL)
	{
		for (int i = 0; i <= depth; i++) printf("  ");
		printf("-> Right:\n");
		print_ast(node->right, depth + 1);
	}
}
