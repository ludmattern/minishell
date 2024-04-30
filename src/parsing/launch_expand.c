/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 10:34:42 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/30 16:00:28 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

bool	redirection_outside_quotes(const char *args)
{
	bool		in_single_quote;
	bool		in_double_quote;
	const char	*cursor;

	in_single_quote = false;
	in_double_quote = false;
	cursor = args;
	while (*cursor != '\0')
	{
		if (*cursor == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*cursor == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if ((*cursor == '>' || *cursor == '<') \
			&& !in_single_quote && !in_double_quote)
			return (true);
		cursor++;
	}
	return (false);
}

void	process_redirections_and_filenames(t_token \
	*token, t_g_data *g_data)
{
	char	*tmp;

	if (redirection_outside_quotes(token->value))
	{
		token->io_list = parse_io_from_command(token->value, \
		token->g_data);
		if (g_heredoc_sigint == 2)
			return ;
		tmp = ft_strdup(token->value);
		free(token->value);
		token->value = del_redir(tmp, 0, 0, g_data);
		free(tmp);
	}
}

void	handle_expanded_token(t_token *token, int last_exit_status)
{
	if ((token->value[0] == -1) || !token->value[0])
	{
		token->is_empty = true;
		token->expanded = malloc(sizeof(char *) * 2);
		token->expanded[0] = ft_strdup("EMPTY");
		if (!token->expanded[0])
			fail_exit_shell(token->g_data);
		token->expanded[1] = NULL;
	}
	else
	{
		token->is_empty = false;
		token->is_export = false;
		if (ft_strncmp(token->value, "export", 6) == 0 && \
		(token->value[6] == ' ' || token->value[6] == '\0'))
   			token->is_export = true;
		token->expanded = expander(token->value, \
		last_exit_status, token->g_data, token->is_export);
		if (token->is_export == true)
			token->is_export = false;
	}
}

void	expe(t_token *lexed, int last_exit_status, t_g_data *g_data)
{
	t_token	*fir;

	fir = lexed;
	while (lexed != NULL)
	{
		lexed->first = fir;
		if (lexed->type == T_WORD)
		{
			process_redirections_and_filenames(lexed, g_data);
			if (g_heredoc_sigint == 2)
				return ;
			lexed->is_add_local = check_local(lexed->value);
			handle_expanded_token(lexed, last_exit_status);
		}
		lexed = lexed->next;
	}
}

void	launch_expand(t_g_data *g_data)
{
	expe(g_data->lexed, g_data->last_exit_status, g_data);
	if (g_heredoc_sigint == 2)
		return ;
}
