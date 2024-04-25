/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   parse.c											:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: fprevot <fprevot@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/04/19 14:53:50 by fprevot		   #+#	#+#			 */
/*   Updated: 2024/04/25 17:32:06 by fprevot		  ###   ########.fr	   */
/*																			*/
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
			return (is_pipe(c, root, g_data));
		else if ((*c)->type == T_PIPE)
			return (is_pipe(c, root, g_data));
		else if ((*c)->type == T_WORD)
			handle_command_node(c, &root, g_data);
		*c = (*c)->next;
	}
	return (root);
}
*/
/*
void	handle_command_node(t_token **current_lex, t_node **root, t_node **current_node, t_g_data *g_data)
{
	if (!(*root))
	{
		*root = create_command_node(*current_lex, g_data);
		current_node = root;
	}
	else
		*current_node = create_command_node(*current_lex, g_data);
}*/
/*void	put_top(t_token **current_lex, t_node **root, t_g_data *g_data)
{
	t_node *tmp;
	
	tmp = *root;
	*root = create_operator_node(*current_lex, g_data);
	(*root)->left = tmp;
	*current_lex = (*current_lex)->next;
	build_ast(current_lex, root, &(*root)->right, g_data);
}*/
/*
void	put_down_right(t_token **current_lex, t_node **root, t_node **current_node, t_g_data *g_data)
{
	t_node *tmp;
	//printf("befor ; %s\n", (*current_lex)->value);
	tmp = create_operator_node(*current_lex, g_data);
	tmp->left = *current_node;
	*current_node = tmp;
	//printf("after ; %s\n", (*current_node)->args);
	*current_lex = (*current_lex)->next;
	build_ast(current_lex, root, &(*current_node)->right, g_data);	
}

void	handle_operator_node(t_token **current_lex, t_node **root, t_node **current_node, t_g_data *g_data)
{
	if ((*root)->type == N_OR || (*root)->type == N_AND || (*current_lex)->type == T_PIPE)
	{
		put_down_right(current_lex, root, current_node, g_data);
	}
	else
		put_top(current_lex, root, g_data);
}*/

t_node *build_ast(t_token **c, t_g_data *g_data)
{
	if (!*c)
		return (NULL);
	t_node *root = NULL;
	t_node *current = NULL;
	t_node *new_node = NULL;
	t_node *pipe_node;
	while (*c && (*c)->type != T_AND && (*c)->type != T_OR) 
	{
		if ((*c)->type == T_WORD || (*c)->type == T_PIPE)
		{
			if ((*c)->type == T_WORD)
				new_node = create_command_node(*c, g_data);
			//else
			//	new_node = create_operator_node(*c, g_data);
			if (!root)
				root = new_node;
			else if (current && current->type == T_PIPE)
				current->left = new_node;
			else if  ((*c)->type == T_WORD)
			{
				pipe_node = create_operator_node(&(t_token){.type = T_PIPE, .value = "|"}, g_data);
				pipe_node->right = root;
				pipe_node->left = new_node;
				root = pipe_node;
			}
			current = new_node;
		}
		*c = (*c)->prev;
	}
	if (*c && ((*c)->type == T_AND || (*c)->type == T_OR))
	{
		t_node *op_node = create_operator_node(*c, g_data);
		op_node->right = root;
		*c = (*c)->prev;
		op_node->left = build_ast(c, g_data);
		root = op_node;
	}
	return (root);
}
