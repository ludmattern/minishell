/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:53:50 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/19 14:55:01 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

t_node	*is_op(t_token **current, t_node *root, int \
last_exit_status, t_g_data *g_data)
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

t_node	*handle_rpar(t_token **c, int last_exit_status, t_g_data *g_data)
{
	t_node	*root;

	*c = (*c)->prev;
	root = build_ast(c, last_exit_status, g_data);
	if (!root)
		return (NULL);
	return (root);
}

void	handle_command_node(t_token **c, t_node **root, \
int last_exit_status, t_g_data *g_data)
{
	if (!(*root))
		*root = create_command_node((*c), last_exit_status, g_data);
	*c = (*c)->prev;
}

t_node	*build_ast(t_token **c, int last_exit_status, t_g_data *g_data)
{
	t_node	*root;

	root = NULL;
	while (*c != NULL)
	{
		if ((*c)->type == T_RPAR)
		{
			root = handle_rpar(c, last_exit_status, g_data);
			if (!root)
				return (NULL);
		}
		else if ((*c)->type == T_LPAR)
			return (*c = (*c)->prev, root);
		else if ((*c)->type == T_AND || (*c)->type == \
		T_OR || (*c)->type == T_PIPE)
			return (is_op(c, root, last_exit_status, g_data));
		else if ((*c)->type == T_WORD)
		{
			handle_command_node(c, &root, last_exit_status, g_data);
		}
		else
			*c = (*c)->prev;
	}
	return (root);
}
