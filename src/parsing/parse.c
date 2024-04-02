/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:10:24 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/29 15:05:22 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

t_node	*is_op(t_token **current, t_node *root, int last_exit_status)
{
	t_node	*nroot;

	nroot = create_operator_node((*current));
	if (!nroot)
	{
		return (NULL);
	}
	last_exit_status++;
	*current = (*current)->next;
	nroot->left = root;
	nroot->right = build_ast(current, last_exit_status);
	if (!nroot->right)
	{
		return (NULL);
	}
	return (nroot);
}

t_node	*build_ast(t_token **current, int last_exit_status)
{
	t_node	*root;

	root = NULL;
	while (*current != NULL)
	{
		if ((*current)->type == 9)
		{
			*current = (*current)->next;
			root = build_ast(current, last_exit_status);
			if (!root)
			{
				return (NULL);
			}
		}
		else if ((*current)->type == 10)
			return (*current = (*current)->next, root);
		else if ((*current)->type == T_AND || (*current)->type == \
			T_OR || (*current)->type == T_PIPE)
			return (is_op(current, root, last_exit_status));
		else if ((*current)->type == T_WORD)
		{
			if (!root)
				root = create_command_node((*current), last_exit_status);
			*current = (*current)->next;
		}
		else
			*current = (*current)->next;
	}
	return (root);
}
