/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:53:50 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/24 19:20:34 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"
/*
t_node	*is_pipe(t_token **current, t_node *root, t_g_data *g_data)
{
	t_node	*nroot;

	nroot = create_operator_node((*current), g_data);
	if (!nroot)
		return (NULL);
	*current = (*current)->next;
	nroot->right = build_ast(current, g_data);
	if (!nroot->right)
		return (NULL);
	nroot->left = root;
	return (nroot);
}
t_node	*handle_rpar(t_token **c, t_g_data *g_data)
{
	t_node	*root;

	*c = (*c)->next;
	root = build_ast(c, g_data);
	if (!root)
		return (NULL);
	return (root);
}

void	handle_command_node(t_token **c, t_node **root, t_g_data *g_data)
{
	if (!(*root))
		*root = create_command_node((*c), g_data);
	else
		(*root)->left = create_command_node((*c), g_data);
}

t_node	*build_ast(t_token **c, t_g_data *g_data)
{
	t_node	*root;

	root = NULL;
	while (*c != NULL)
	{
		if ((*c)->type == T_RPAR)
		{
			root = handle_rpar(c, g_data);
			if (!root)
				return (NULL);
		}
		else if ((*c)->type == T_LPAR)
			return (*c = (*c)->next, root);
		else if ((*c)->type == T_AND || (*c)->type == T_OR)
			return (is_op(c, root, g_data));
		else if ((*c)->type == T_PIPE)
			return (is_pipe(c, root, g_data));
		else if ((*c)->type == T_WORD)
			handle_command_node(c, &root, g_data);
		*c = (*c)->next;
	}
	return (root);
}
*/

void	handle_command_node(t_token **current_lex, t_node **root, t_node **current_node, t_g_data *g_data)
{
	if (!(*root))
	{
		*root = create_command_node(*current_lex, g_data);
		current_node = root;
	}
	else
		*current_node = create_command_node(*current_lex, g_data);
}
void	put_top(t_token **current_lex, t_node **root, t_g_data *g_data)
{
	t_node *tmp;
	
	tmp = *root;
	*root = create_operator_node(*current_lex, g_data);
	(*root)->left = tmp;
	*current_lex = (*current_lex)->next;
	build_ast(current_lex, root, &(*root)->right, g_data);
}

void	put_down_right(t_token **current_lex, t_node **root, t_node **current_node, t_g_data *g_data)
{
	t_node *tmp;
	//printfast
	tmp = create_operator_node(*current_lex, g_data);
	tmp->left = *current_node;
	*current_node = tmp;
	*current_lex = (*current_lex)->next;
	build_ast(current_lex, root, &(*current_node)->right, g_data);	
}

void	handle_operator_node(t_token **current_lex, t_node **root, t_node **current_node, t_g_data *g_data)
{
	if ((*root)->type == N_OR || (*root)->type == N_AND || (*current_lex)->type == T_PIPE)
	{
		printf("%s\n", (*current_node)->args);
		put_down_right(current_lex, root, current_node, g_data);
	}
	else
		put_top(current_lex, root, g_data);
}

void	handle_pipe_node(t_token **current_lex, t_node **root, t_node **current_node, t_g_data *g_data)
{
	put_down_right(current_lex, root, current_node, g_data);	
}

void	build_ast(t_token **current_lex, t_node **root, t_node **current_node, t_g_data *g_data)
{
	
	if ((*current_lex)->type == T_WORD)
		handle_command_node(current_lex, root, current_node, g_data);
	else if ((*current_lex)->type == T_OR || (*current_lex)->type == T_AND || (*current_lex)->type == T_PIPE)
		handle_operator_node(current_lex, root, current_node, g_data);
	
}
