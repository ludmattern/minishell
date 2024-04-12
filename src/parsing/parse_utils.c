/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 16:19:08 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/12 12:54:01 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

t_node	*create_empty_node(t_token *tkn, t_g_data *g_data)
{
	t_node	*new;

	new = malloc(sizeof(t_node) * 1);
	if(!new)
		fail_exit_shell(g_data);
	ft_memset(new, 0, sizeof(t_node));
	new->args = ft_strdup(tkn->value);
	if(!new->args)
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

char *del_redir(char *cmd, int i, int j, t_g_data *g_data)
{
	char *result = malloc(strlen(cmd) + 1);
	if(!result)
		fail_exit_shell(g_data);
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
			while (ft_isspace((unsigned char)cmd[i])) 
				i++;
			while (cmd[i] != '\0' && (inq || (!ft_isspace((unsigned char)cmd[i]) && cmd[i] != '>' && cmd[i] != '<')))
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
			while (ft_isspace((unsigned char)cmd[i])) 
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

char **ft_cleaner(char **args,  t_g_data *g_data)
{
	int i = 0;
	int j = 0;
	int k = 0;
	char **result = malloc(sizeof(char *) * (ft_arrlen(args) + 1));
	if(!result)
		fail_exit_shell(g_data);
	while (args[i] != NULL)
	{
		if (args[i][0] != '\0')
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
}

bool check_local(char *arg)
{
	int i = 0;

	while (arg && arg[i])
	{
		if (arg[i] == '=' && arg[i + 1] != ' ' && arg[i - 1] != ' ')
			return (true);
		i++;
	}
	return (false);
}



t_node *create_command_node(t_token *tkn, int last_exit_status, t_g_data *g_data)
{
	t_node *node;

	(void)last_exit_status;
	node = create_empty_node(tkn, g_data);
	node->io_list = tkn->io_list;
	node->expanded_args = tkn->expanded;
	if (node->expanded_args[0] != NULL)
	{
		node->command_path = get_command_path(node->expanded_args[0]);
	}
	node->is_add_local = check_local(node->command_path);
	
	//print_exp(node->expanded_args, node->args);
	//printredir(node->io_list);
	node->type = N_CMD;
	return (node);
}

void	expe(t_token *lexed, int last_exit_status, t_g_data *g_data)
{
	t_token *fir;
	char *tmp;
	fir = lexed;
    while (lexed != NULL)
    {
		lexed->first = fir;
		if (lexed->type == T_WORD)
		{
			if (redirection_outside_quotes(lexed->value)) 
			{
				lexed->io_list = parse_io_from_command(lexed->value, last_exit_status, lexed->g_data);
				if (g_heredoc_sigint == 2)
					return ;
				tmp = ft_strdup(lexed->value);
				free(lexed->value);
				lexed->value = del_redir(tmp, 0, 0, g_data);
				free(tmp);
				//printf("\n%s\n", node->args);
			}
			lexed->expanded = expander(lexed->value, last_exit_status, lexed->g_data);
			lexed->expanded = ft_cleaner(lexed->expanded, g_data);
		}
		//printredir(lexed->io_list);
        lexed = lexed->next;
    }
}


void	launch_expand(t_g_data *g_data)
{
    expe(g_data->lexed, g_data->last_exit_status, g_data);
	if (g_heredoc_sigint == 2)
			return ;
}