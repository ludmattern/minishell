/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expan_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 21:40:22 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/05 21:41:28 by fprevot          ###   ########.fr       */
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

void	free_string_array(char **array)
{
	char	**temp;

	if (array)
	{
		temp = array;
		while (*temp)
		{
			free(*temp);
			temp++;
		}
		free(array);
	}
}

void	free_io_nod(t_io_node *node)
{
	if (node)
	{
		free(node->value);
		free_string_array(node->expanded_value);
		free(node);
	}
}

void	free_io_list(t_io_node *head)
{
	t_io_node	*current;
	t_io_node	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		free_io_nod(current);
		current = next;
	}
}
