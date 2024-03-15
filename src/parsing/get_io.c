/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_io.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:49:43 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/13 17:54:07 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

t_io_node	*create_io_node(t_io_type type, t_token *tkn, int last_exit_status)
{
	t_io_node	*io;

	io = malloc(sizeof(t_io_node));
	if (io == NULL)
		return (NULL);
	io->type = type;
	if (tkn->next != NULL)
		io->value = ft_strdup(tkn->next->next->value);
	io->expanded_value = expander(io->value, last_exit_status);
	io->here_doc = 0;
	io->next = NULL;
	return (io);
}

void	add_io_node(t_io_node **head, t_io_node **tail, \
		t_token **tkn, int last_exit_status)
{
	t_io_node	*new_io;

	new_io = NULL;
	if ((*tkn)->next->type == T_GREAT)
		new_io = create_io_node(IO_OUT, *tkn, last_exit_status);
	else if ((*tkn)->next->type == T_LESS)
		new_io = create_io_node(IO_IN, *tkn, last_exit_status);
	else if ((*tkn)->next->type == T_DLESS)
		new_io = create_io_node(IO_HEREDOC, *tkn, last_exit_status);
	else if ((*tkn)->next->type == T_DGREAT)
		new_io = create_io_node(IO_APPEND, *tkn, last_exit_status);
	if (new_io != NULL)
	{
		if (*head == NULL)
		{
			*head = new_io;
			*tail = new_io;
		}
		else
		{
			(*tail)->next = new_io;
			new_io->prev = *tail;
			*tail = new_io;
		}
	}
}

t_io_node	*make_io(t_token **tkn, int last_exit_status)
{
	t_io_node	*head;
	t_io_node	*tail;

	head = NULL;
	tail = NULL;
	while (*tkn != NULL && (*tkn)->next != NULL && \
			((*tkn)->next->type == T_GREAT || (*tkn)->next->type == T_LESS || \
			(*tkn)->next->type == T_DLESS || (*tkn)->next->type == T_DGREAT))
	{
		add_io_node(&head, &tail, tkn, last_exit_status);
		*tkn = (*tkn)->next->next;
	}
	return (head);
}
