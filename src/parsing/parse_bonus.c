/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 09:06:27 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/13 10:24:52 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/bonus/parse_bonus.h"

t_node	*process_token_type(t_token **c, t_g_data *g_data, \
t_node **root, t_node **current)
{
	t_node	*new_node;

	new_node = NULL;
	if ((*c)->type == T_RPAR)
		new_node = process_rpar(c, g_data);
	else if ((*c)->type == T_LPAR)
	{
		g_data->checkpar = 1;
		return (NULL);
	}
	else if ((*c)->type == T_WORD)
		new_node = create_command_node(*c, g_data);
	if (!(*root))
		*root = new_node;
	else if (*current && (*current)->type == N_PIPE)
		(*current)->left = new_node;
	else if (new_node)
		*root = add_pipe_node(new_node, root, g_data);
	return (new_node);
}

t_node	*build_ast(t_token **c, t_g_data *g_data)
{
	t_node	*root;
	t_node	*current;
	t_node	*new_node;

	root = NULL;
	current = NULL;
	new_node = NULL;
	g_data->checkpar = 0;
	while (*c != NULL && (*c)->type != T_AND && (*c)->type != T_OR)
	{
		if ((*c)->type == T_WORD || (*c)->type == T_PIPE \
		|| (*c)->type == T_RPAR || (*c)->type == T_LPAR)
		{
			new_node = process_token_type(c, g_data, &root, &current);
			if (g_data->checkpar == 1)
				break ;
		}
		move_token_prev(c);
	}
	root = handle_op(c, g_data, root);
	return (move_token_prev(c), root);
}
