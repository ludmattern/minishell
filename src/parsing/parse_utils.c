/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:19:08 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/17 12:40:43 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

t_node	*create_empty_node(t_token *tkn, t_g_data *g_data)
{
	t_node	*new;

	new = malloc(sizeof(t_node) * 1);
	if (!new)
		fail_exit_shell(g_data);
	ft_memset(new, 0, sizeof(t_node));
	new->args = ft_strdup(tkn->value);
	if (!new->args)
		fail_exit_shell(g_data);
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
/*
char	**ft_cleaner(char **args,  t_g_data *g_data)
{
	int i = 0;
	int j = 0;
	int k = 0;
	char **result = malloc(sizeof(char *) * (ft_arrlen(args) + 1));
	if(!result)
		fail_exit_shell(g_data);
	while (args[i] != NULL)
	{
		if (args[i][0] != '\0' )
		{
			if (i == 0)
			{
				k = 0;
				while (args[i] && args[i][k] != '\0')
					k++;
				if (args[0][k - 1] != '=')
					result[j++] = ft_strdup(args[i]);
			}
			else
				result[j++] = ft_strdup(args[i]);
			if(!result)
				fail_exit_shell(g_data);
		}
		i++;
	}
	result[j] = NULL;
	i = 0;
	ft_free_double_array(args);
	return (result);
}*/

bool	check_local(char *arg)
{
	int	i;

	i = 0;
	while (arg && arg[i] && arg[i] != ' ')
	{
		if (arg[i] == '=' && arg[i + 1] != ' ' && arg[i - 1] != ' ')
			return (true);
		i++;
	}
	return (false);
}

t_node	*create_command_node(t_token *tkn, int \
last_exit_status, t_g_data *g_data)
{
	t_node	*node;

	(void)last_exit_status;
	node = create_empty_node(tkn, g_data);
	node->io_list = tkn->io_list;
	node->expanded_args = tkn->expanded;
	if (node->expanded_args[0] != NULL)
		node->command_path = get_command_path(node->expanded_args[0], g_data);
	if (g_data->lexed->is_add_local == true)
		node->is_add_local = true;
	else
		node->is_add_local = false;
	if (g_data->lexed->is_empty == true)
		node->is_empty = true;
	else
		node->is_empty = false;
	node->type = N_CMD;
	return (node);
}
