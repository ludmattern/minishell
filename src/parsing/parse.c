/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:10:24 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/16 14:54:20 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"






t_node	*is_op(t_token **current, t_node *root, int last_exit_status, t_g_data *g_data)
{
	t_node	*nroot;

	nroot = create_operator_node((*current), g_data);
	if (!nroot)
		return (NULL);
	last_exit_status++;
	*current = (*current)->prev;
	nroot->right = root;
	nroot->left = build_ast(current, last_exit_status, g_data);
	if (!nroot->left)
		return (NULL);
	return (nroot);
}

t_node	*build_ast(t_token **current, int last_exit_status, t_g_data *g_data)
{
	t_node	*root;

	root = NULL;
	while (*current != NULL)
	{
		if ((*current)->type == T_RPAR)
		{
			*current = (*current)->prev;
			root = build_ast(current, last_exit_status, g_data);
			if (!root)
				return (NULL);
		}
		else if ((*current)->type == T_LPAR)
			return (*current = (*current)->prev, root);
		else if ((*current)->type == T_AND || (*current)->type == \
			T_OR || (*current)->type == T_PIPE)
			return (is_op(current, root, last_exit_status, g_data));
		else if ((*current)->type == T_WORD)
		{
			if (!root)
				root = create_command_node((*current), last_exit_status, g_data);
			*current = (*current)->prev;
		}
		else
			*current = (*current)->prev;
	}
	return (root);
}
