/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:04:04 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/21 17:59:57 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"
#include "../../inc/exec.h"

t_io_node	*create_io_node_from_string(t_io_type type, \
char *value, int last_exit_status, t_g_data *data)
{
	t_io_node	*io;

	io = malloc(sizeof(t_io_node));
	if (!io)
		return (NULL);
	memset(io, 0, sizeof(t_io_node));
	io->type = type;
	if (io->type == IO_HEREDOC)
	{
		read_heredoc_into_string(value, &io->value);
		if (g_heredoc_sigint == 2)
			return (NULL);
	}
	else
		io->value = ft_strdup(value);
	if (!io->value)
		return (free(io), NULL);
	if (io->type == IO_HEREDOC)
		io->expanded_value = replace_input_vars(data, io->value, 0);
	else
		io->expanded_value = expander(io->value, last_exit_status, data);
	io->here_doc = 0;
	io->prev = NULL;
	io->next = NULL;
	return (io);
}

t_io_node	*parse_io_from_command(char *cmd, int \
last_exit_status, t_g_data *data)
{
	t_io_bundle	io;

	io.head = NULL;
	io.tail = NULL;
	io.current_quote = 0;
	io.cursor = cmd;
	while (*io.cursor)
	{
		if ((*io.cursor == '"' || *io.cursor == '\'') && !io.current_quote)
			io.current_quote = *io.cursor;
		else if (*io.cursor == io.current_quote)
			io.current_quote = 0;
		else if ((*io.cursor == '>' || *io.cursor == '<') && !io.current_quote)
		{
			set_io_type(&io.type, &io.cursor);
			add_new_io_node(&io, &io.cursor, last_exit_status, data);
			if (g_heredoc_sigint == 2)
				return (NULL);
		}
		if (io.cursor >= cmd + ft_strlen(cmd))
			break ;
		io.cursor++;
	}
	return (io.head);
}
