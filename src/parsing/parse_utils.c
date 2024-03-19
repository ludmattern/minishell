/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:19:08 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/18 12:57:45 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

t_node	*create_empty_node(t_token *tkn)
{
	t_node	*new;

	new = malloc(sizeof(t_node) * 1);
	new->args = ft_strdup(tkn->value);
	new->expanded_args = NULL;
	new->command_path = NULL;
	new->io_list = NULL;
	new->right = NULL;
	new->left = NULL;
	new->type = N_INIT;
	return (new);
}

t_node	*create_operator_node(t_token *tkn)
{
	t_node	*node;

	node = create_empty_node(tkn);
	if (tkn->type == T_PIPE)
		node->type = N_PIPE;
	else if (tkn->type == T_AND)
		node->type = N_AND;
	else if (tkn->type == T_OR)
		node->type = N_OR;
	return (node);
}

t_node	*create_command_node(t_token *tkn, int last_exit_status)
{
	t_node	*node;

	node = create_empty_node(tkn);
	node->expanded_args = expander(node->args, last_exit_status);
	node->command_path = (get_command_path(node->expanded_args[0]));
	node->type = N_CMD;
	if (tkn->next != NULL && (tkn->next->type == T_DGREAT || tkn->next->type == \
		T_DLESS || tkn->next->type == T_GREAT || tkn->next->type == T_LESS))
		node->io_list = make_io(&tkn, last_exit_status);
	return (node);
}
