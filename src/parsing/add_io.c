/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   add_io.c										   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: fprevot <fprevot@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/04/17 15:50:42 by fprevot		   #+#	#+#			 */
/*   Updated: 2024/04/17 15:57:25 by fprevot		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../../inc/parse.h"

void	skip_whitespace(char **cursor)
{
	while (**cursor == ' ')
		(*cursor)++;
}

char	*extract_filename(char **cursor)
{
	char	*start;
	char	c;

	c = ' ';
	if (**cursor == '"')
	{
		c = '"';
		start = (*cursor)++;
	}
	else
		start = *cursor;
	while (**cursor && **cursor != c && **cursor != '>' && **cursor != '<')
		(*cursor)++;
	return (ft_strndup(start, *cursor - start));
}

void	initialize_and_add_io_node(t_io_bundle *io, t_io_node *new_io)
{
	if (!io->head)
	{
		io->head = new_io;
		io->tail = new_io;
	}
	else
	{
		io->tail->next = new_io;
		new_io->prev = io->tail;
		io->tail = new_io;
	}
}

void	add_new_io_node(t_io_bundle *io, char **cursor, \
t_g_data *data)
{
	t_io_node	*new_io;
	char		*filename;

	skip_whitespace(cursor);
	if (countq(*cursor) > 2 && **cursor == '"')
		filename = extract_with_quote(cursor, data);
	else
		filename = extract_filename(cursor);
	if (!filename)
		fail_exit_shell(data);
	new_io = create_io_node_from_string(io->type, filename, \
	data);
	if (g_heredoc_sigint == 2)
		return ;
	if (new_io == NULL)
	{
		free(filename);
		fail_exit_shell(data);
	}
	free(filename);
	initialize_and_add_io_node(io, new_io);
}
