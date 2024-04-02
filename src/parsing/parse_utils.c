/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:19:08 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/02 11:52:59 by fprevot          ###   ########.fr       */
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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>


char *del_redir(const char *cmd) {
    char *result = malloc(strlen(cmd) + 1);
    if (!result) return NULL;
    int i = 0, j = 0;
    int inQuotes = 0; 
    char currentQuote = 0;

    while (cmd[i] != '\0') {
        if ((cmd[i] == '\'' || cmd[i] == '\"') && (i == 0 || cmd[i - 1] != '\\'))
        {
            if (!inQuotes) 
            {
                inQuotes = 1;
                currentQuote = cmd[i];
            } 
            else if (cmd[i] == currentQuote)
                inQuotes = 0;
        }
        if (inQuotes) 
        {
            result[j++] = cmd[i++];
            continue;
        }

        if (cmd[i] == '>' || cmd[i] == '<') {
            while (cmd[i] == '>' || cmd[i] == '<') 
                i++;
            while (isspace((unsigned char)cmd[i])) 
                i++;
            while (cmd[i] != '\0' && !isspace((unsigned char)cmd[i])) 
                i++;
            while (isspace((unsigned char)cmd[i])) 
                i++;
        }
        else
            result[j++] = cmd[i++];
    }
    result[j] = '\0';
    return result;
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

t_node *create_command_node(t_token *tkn, int last_exit_status)
{
    t_node *node;
    
    node = create_empty_node(tkn);
    if (redirection_outside_quotes(node->args)) 
    {
        node->io_list = parse_io_from_command(node->args, last_exit_status);
        node->args = del_redir(node->args);
        //printf("\n%s\n", node->args);
        
    }
    node->expanded_args = expander(node->args, last_exit_status);
    node->command_path = get_command_path(node->expanded_args[0]);
    //print_exp(node->expanded_args, node->args);
    //printredir(node->io_list);
    node->type = N_CMD;

    return (node);
}

