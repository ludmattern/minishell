/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:19:08 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/08 11:55:09 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

t_node	*create_empty_node(t_token *tkn, t_g_data *g_data)
{
	t_node	*new;

	new = malloc(sizeof(t_node) * 1);
	if (!new)
	{
		ft_free_double_array(tkn->expanded);
		free_lexed(g_data->lexed);
		fail_exit_shell(g_data);
	}
	memset(new, 0, sizeof(t_node));
	new->args = ft_strdup(tkn->value);
	if (!new->args)
	{
		free(new);
		ft_free_double_array(tkn->expanded);
		free_lexed(g_data->lexed);
		fail_exit_shell(g_data);
	}
	new->type = N_INIT;
	return (new);
}

t_node	*create_operator_node(t_token *tkn, t_g_data *g_data)
{
	t_node	*node;

	node = create_empty_node(tkn, g_data);
	if (tkn->type == T_PIPE)
		node->type = N_PIPE;
	else if (tkn->type == T_AND)
		node->type = N_AND;
	else if (tkn->type == T_OR)
		node->type = N_OR;
	return (node);
}

size_t	ft_arrlen(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i] != NULL)
		i++;
	return (i);
}

bool	check_local(char *arg)
{
	int	i;

	i = 0;
	if (!ft_isalpha(arg[0]))
		return (false);
	while (arg && arg[i] && arg[i] != ' ')
	{
		if (arg[i] == '=' && arg[i + 1] != ' ' && arg[i - 1] != ' ')
			return (true);
		i++;
	}
	return (false);
}

t_node	*create_command_node(t_token *tkn, t_g_data *g_data)
{
	t_node	*node;

	node = create_empty_node(tkn, g_data);
	node->io_list = tkn->io_list;
	node->expanded_args = tkn->expanded;
	if (node->expanded_args[0] != NULL)
		node->command_path = get_command_path(node->expanded_args[0], g_data);
	if (tkn->is_add_local == true)
		node->is_add_local = true;
	else
		node->is_add_local = false;
	if (tkn->is_empty == true)
	{
		node->is_empty = true;
	}
	else
		node->is_empty = false;
	node->type = N_CMD;
	return (node);
}
