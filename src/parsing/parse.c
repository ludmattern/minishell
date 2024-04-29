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

t_node *build_ast(t_token **c, t_g_data *g_data)
{
	t_node *root = NULL;
	t_node *current = NULL;
	t_node *new_node = NULL;
	t_node *pipe_node = NULL;

	g_data->checkpar = 0;
	while (*c != NULL && (*c)->type != T_AND && (*c)->type != T_OR) 
	{
		if ((*c)->type == T_WORD || (*c)->type == T_PIPE || (*c)->type == T_RPAR || (*c)->type == T_LPAR)
		{
			if ((*c)->type == T_RPAR)
			{
				*c = (*c)->prev;
				new_node = build_ast(c, g_data);
			}
			else if ((*c)->type == T_LPAR)
			{
				g_data->checkpar = 1;
				break ;
			}
			else if ((*c)->type == T_WORD)
				new_node = create_command_node(*c, g_data);
			if (!root)
				root = new_node;
			else if (current->type == N_PIPE)
				current->left = new_node;
			else if  ((*c)->type == T_WORD || g_data->checkpar == 1)
			{
				pipe_node = create_operator_node(&(t_token){.type = T_PIPE, .value = "|"}, g_data);
				pipe_node->right = root;
				pipe_node->left = new_node;
				root = pipe_node;
				g_data->checkpar = 0;
			}
			current = new_node;
		}
		if ((*c)->type != T_AND && (*c)->type != T_OR)
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
	if (*c && (*c)->type == T_LPAR && (*c)->prev != NULL)
		*c = (*c)->prev;
	return (root);
}
