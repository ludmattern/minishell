/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:40:08 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/16 14:24:51 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"


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
			lexed->is_add_local = check_local(lexed->value);
			
				
			if (!lexed->value[0] && lexed->io_list != NULL)
			{
				lexed->is_empty = true;
				lexed->expanded = malloc(sizeof(char *) * 2);
				lexed->expanded[0] = ft_strdup("EMPTY");
				lexed->expanded[1] = NULL;
			}
			else
				lexed->expanded = expander(lexed->value, last_exit_status, lexed->g_data);
			

			//lexed->expanded = ft_cleaner(lexed->expanded, g_data);
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