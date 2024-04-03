/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:19:08 by fprevot           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/04/03 13:23:53 by fprevot          ###   ########.fr       */
=======
/*   Updated: 2024/04/03 15:42:38 by lmattern         ###   ########.fr       */
>>>>>>> 5c79883a5694c02395415fd7871df39179f80ef4
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

t_node	*create_empty_node(t_token *tkn)
{
	t_node	*new;

	new = malloc(sizeof(t_node) * 1);
	ft_memset(new, 0, sizeof(t_node));
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

char *del_redir(char *cmd, int i, int j)
{
	char *result = malloc(strlen(cmd) + 1);
	if (!result) 
		return (NULL);
	int inq = 0;
	char currentq = 0;

	while (cmd[i] != '\0') {
		if ((cmd[i] == '\'' || cmd[i] == '\"') && (inq == 0 || currentq == cmd[i])) 
		{
			inq = !inq;
			if (inq)
				currentq = cmd[i];
			else 
				currentq = 0;
		} 
		else if (!inq && (cmd[i] == '>' || cmd[i] == '<'))
		{
			while (cmd[i] == '>' || cmd[i] == '<') 
				i++;
			while (isspace((unsigned char)cmd[i])) 
				i++;
			while (cmd[i] != '\0' && (inq || (!isspace((unsigned char)cmd[i]) && cmd[i] != '>' && cmd[i] != '<')))
			{
				if (cmd[i] == currentq)
				{
					inq = 0;
					currentq = 0;
				} 
				else if ((cmd[i] == '\'' || cmd[i] == '\"') && !inq)
				{
					inq = 1;
					currentq = cmd[i];
				}
				i++;
			}
			while (isspace((unsigned char)cmd[i])) 
				i++;
			continue;
		}
		result[j++] = cmd[i++];
	}
	result[j] = '\0'; 
	return (result);
}

bool redirection_outside_quotes(const char *args)
{
	bool in_single_quote = false;
	bool in_double_quote = false;
	const char *cursor = args;

	while (*cursor != '\0') 
	{
		if (*cursor == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*cursor == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if ((*cursor == '>' || *cursor == '<') && !in_single_quote && !in_double_quote)
			return (true);
		cursor++;
	}
	return (false);
}

size_t ft_arrlen(char **arr)
{
	size_t i = 0;
	while (arr[i] != NULL)
		i++;
	return (i);
}

char **ft_cleaner(char **args)
{
	int i = 0;
	int j = 0;
	char **result = malloc(sizeof(char *) * (ft_arrlen(args) + 1));
	if (!result)
		return (NULL);
	while (args[i] != NULL)
	{
		if (args[i][0] != '\0')
			result[j++] = ft_strdup(args[i]);
		i++;
	}
	result[j] = NULL;
	i = 0;
	while (args[i] != NULL)
		free(args[i++]);
	free(args);
	return (result);
}

t_node *create_command_node(t_token *tkn, int last_exit_status)
{
	t_node *node;
	node->is_add_local = add_local_boot(node);
	node = create_empty_node(tkn);
	if (redirection_outside_quotes(node->args)) 
	{
		node->io_list = parse_io_from_command(node->args, last_exit_status);
		node->args = del_redir(node->args, 0, 0);
		//printf("\n%s\n", node->args);
	}
	node->expanded_args = expander(node->args, last_exit_status);
	node->expanded_args = ft_cleaner(node->expanded_args);
	if (node->expanded_args[0] != NULL)
		node->command_path = get_command_path(node->expanded_args[0]);
	//print_exp(node->expanded_args, node->args);
	//printredir(node->io_list);
	node->type = N_CMD;
	return (node);
}

