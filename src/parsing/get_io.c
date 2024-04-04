/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 15:13:47 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/04 11:15:32 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

t_io_node *create_io_node_from_string(t_io_type type, char *value, int last_exit_status, t_global_data *data) 
{
	t_io_node *io = malloc(sizeof(t_io_node));
	if (!io) 
	return NULL;
	io->type = type;
	io->value = ft_strdup(value);
	io->expanded_value = expander(io->value, last_exit_status, data);
	io->here_doc = 0;
	io->prev = NULL;
	io->next = NULL;
	return io;
}

static void set_io_type(t_io_type *type, char **cursor)
{
	if (**cursor == '>')
	{
		*type = IO_OUT;
		(*cursor)++;
		if (**cursor == '>')
		{
			*type = IO_APPEND;
			(*cursor)++;
		}
	}
	else if (**cursor == '<')
	{
		*type = IO_IN;
		(*cursor)++;
		if (**cursor == '<')
		{
			*type = IO_HEREDOC;
			(*cursor)++;
		}
	}
}

int countq(const char *str) 
{
	int count = 0;
	while (*str && *str != '>' && *str != '<' && *str != ' ') 
	{
		if (*str == '"') 
		{
			count++;
		}
		str++;
	}
	return count;
}

char *extract_with_quote(char **cursor)
{
	char *result = malloc(strlen(*cursor) + 1);
	if (!result) 
		return (NULL);
	char *output = result;
	int insideq = 0;

	while (**cursor && **cursor != '>' && **cursor != '<')
	{
		if (**cursor == '"' && !insideq) 
		{
			insideq = 1;
			(*cursor)++;
		}
		else if (**cursor == '"' && insideq) 
		{
			insideq = 0;
			(*cursor)++;
		}
		else 
		{
			*output++ = **cursor;
			(*cursor)++;
		}
		if (!insideq && **cursor == '"')
		{
			insideq = 1;
			(*cursor)++;
		}
	}
	*output = '\0';
	return (result);
}


static void add_new_io_node(t_io_node **head, t_io_node **tail, \
							t_io_type type, char **cursor, int last_exit_status, t_global_data *data)
{
	char c = ' ';
	char *start;
	char *filename;
	
	while (**cursor == ' ')
		(*cursor)++;
	if (countq(*cursor) > 2 && **cursor == '"')
	{
		filename = extract_with_quote(cursor);
	}
	else
	{
		if (**cursor == '"')
		{
			c = '"';
			start = *cursor;
			(*cursor)++;
		}
		else
			start = *cursor;
		while (**cursor && **cursor != c && **cursor != '>' && **cursor != '<')
			(*cursor)++;
		filename = ft_strndup(start, *cursor - start); 
	}
	t_io_node *new_io = create_io_node_from_string(type, filename, last_exit_status, data);
	free(filename);
	if (!*head)
		*head = *tail = new_io;
	else
	{
		(*tail)->next = new_io;
		new_io->prev = *tail;
		*tail = new_io;
	}
}

t_io_node *parse_io_from_command(char *cmd, int last_exit_status, t_global_data *data)
{
	t_io_node *head = NULL;
	t_io_node *tail = NULL;
	char current_quote = 0;
	char *cursor = cmd;

	while (*cursor)
	{
		if ((*cursor == '"' || *cursor == '\'') && !current_quote)
			current_quote = *cursor;
		else if (*cursor == current_quote)
			current_quote = 0;
		else if ((*cursor == '>' || *cursor == '<') && !current_quote)
		{
			t_io_type type;
			set_io_type(&type, &cursor);
			add_new_io_node(&head, &tail, type, &cursor, last_exit_status, data);
		}
		
		cursor++;
	}
	return (head);
}
