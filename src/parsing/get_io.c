/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:36:16 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/02 13:41:57 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"
#include "../../inc/exec.h"

int	calc_size(char *value)
{
	int	i;
	int	c;

	i = 0;
	c = 0;
	while (value[i])
	{
		if (value[i] != '\'' && value[i] != '"')
			c++;
		i++;
	}
	return (c + 1);
}

char	*value_her(char *value)
{
	char	*res;
	int		i;
	int		j;
	int		size;

	i = 0;
	j = 0;
	size = calc_size(value);
	res = malloc(size);
	if (res == NULL)
		return (NULL);
	while (value[i])
	{
		if (value[i] != '"' && value[i] != '\'')
			res[j++] = value[i];
		i++;
	}
	res[j] = '\0';
	return (res);
}

bool	check_delimiter(char *value)
{
	if (value[0] == '"' || value[0] == '\'')
		return (false);
	return (true);
}

t_io_node	*create_io_node_from_string(t_io_type type, \
char *value, t_g_data *data)
{
	t_io_node	*io;
	//bool		check_her = check_delimiter(value);

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
	if (io->type == IO_HEREDOC/* && check_her == true*/)
		io->expanded_value = replace_input_vars(data, io->value, 0);
	else
	{
		io->expanded_value = replace_input_vars(data, io->value, 0);
	}
	io->here_doc = 0;
	return (io->prev = NULL, io->next = NULL, io);
}

t_io_node	*parse_io_from_command(char *cmd, t_g_data *data)
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
			add_new_io_node(&io, &io.cursor, data);
			if (g_heredoc_sigint == 2)
				return (NULL);
		}
		if (io.cursor >= cmd + ft_strlen(cmd))
			break ;
		io.cursor++;
	}
	
	return (io.head);
}
