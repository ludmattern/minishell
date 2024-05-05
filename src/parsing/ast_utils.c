/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 17:32:58 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/05 20:26:58 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

t_node	*process_operator(t_token **c, t_g_data *g_data, t_node *root)
{
	t_node	*op_node;

	op_node = create_operator_node(*c, g_data);
	op_node->right = root;
	*c = (*c)->prev;
	op_node->left = build_ast(c, g_data);
	return (op_node);
}

t_node	*process_rpar(t_token **c, t_g_data *g_data)
{
	*c = (*c)->prev;
	return (build_ast(c, g_data));
}

t_node	*add_pipe_node(t_node *new_node, t_node **root, t_g_data *g_data)
{
	t_node	*pipe_node;
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
	{
		free_lexed(g_data->lexed);
		free_tree(new_node);
		free_tree(*root);
		fail_exit_shell(g_data);
	}
	node->type = T_PIPE;
	node->value = "|";
	pipe_node = create_operator_node(node, g_data);
	pipe_node->right = *root;
	pipe_node->left = new_node;
	g_data->checkpar = 0;
	free(node);
	return (pipe_node);
}

void	move_token_prev(t_token **c)
{
	if (*c)
	{
		if ((*c)->type != T_AND && (*c)->type != T_OR)
			*c = (*c)->prev;
		else if ((*c)->type == T_LPAR && (*c)->prev != NULL)
			*c = (*c)->prev;
	}
}

t_node	*handle_op(t_token **c, t_g_data *g_data, t_node *root)
{
	if (*c && ((*c)->type == T_AND || (*c)->type == T_OR))
		root = process_operator(c, g_data, root);
	return (root);
}
