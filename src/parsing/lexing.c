/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:39:45 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/13 17:43:45 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

void	fill_value(t_token *lex, int num)
{
	if (num == 1)
		lex->value = "<\0";
	else if (num == 2)
		lex->value = ">\0";
	else if (num == 3)
		lex->value = "<<\0";
	else if (num == 4)
		lex->value = ">>\0";
	else if (num == 5)
		lex->value = "|\0";
	else if (num == 6)
		lex->value = "&&\0";
	else if (num == 7)
		lex->value = "||\0";
	else if (num == 8)
		lex->value = "\n\0";
	else if (num == 9)
		lex->value = "(\0";
	else if (num == 10)
		lex->value = ")\0";
}

void	fill_type(t_token *lex, int num, int *i, int size)
{
	imore(0, size, i);
	if (num == 1)
		lex->type = T_LESS;
	else if (num == 2)
		lex->type = T_GREAT;
	else if (num == 3)
		lex->type = T_DLESS;
	else if (num == 4)
		lex->type = T_DGREAT;
	else if (num == 5)
		lex->type = T_PIPE;
	else if (num == 6)
		lex->type = T_AND;
	else if (num == 7)
		lex->type = T_OR;
	else if (num == 8)
		lex->type = T_NEWLINE;
	else if (num == 9)
		lex->type = 9;
	else if (num == 10)
		lex->type = 10;
	fill_value(lex, num);
}

void	fill_t_word(t_token *lex, int *i, char *in_put)
{
	int		start;
	int		end;
	int		wl;
	int		j;

	while (in_put[*i] == ' ')
		(*i)++;
	start = *i;
	while (in_put[*i] != '|' && in_put[*i] != '<' && in_put[*i] != '>' && \
		!(in_put[*i] == '&' && in_put[*i + 1] == '&') && in_put[*i] != '('\
		&& in_put[*i] != ')' && in_put[*i] != '\0')
	{
		if (in_put[*i] != ' ')
			end = *i;
		(*i)++;
	}
	wl = end - start + 1;
	lex->value = malloc(wl + 1);
	j = -1;
	while (++j < wl)
		lex->value[j] = in_put[start + j];
	lex->value[wl] = '\0';
	lex->type = T_WORD;
}

void	init_filling(t_token *lex, int *i, char *in_put)
{
	while (in_put[*i] == ' ')
		(*i)++;
	if (in_put[*i] == '(')
		fill_type(lex, 9, i, 1);
	else if (in_put[*i] == ')')
		fill_type(lex, 10, i, 1);
	else if (in_put[*i] != '|' && in_put[*i] != '<' && in_put[*i] != '>' && \
		!(in_put[*i] == '&' && in_put[*i + 1] == '&') && in_put[*i] != '(' \
		&& in_put[*i] != ')' && in_put[*i] != '\0')
		fill_t_word(lex, i, in_put);
	else if (in_put[*i] == '|' && in_put[*i + 1] != '|')
		fill_type(lex, 5, i, 1);
	else if (in_put[*i] == '<' && in_put[*i + 1] == '<')
		fill_type(lex, 3, i, 2);
	else if (in_put[*i] == '<' && in_put[*i + 1] != '<')
		fill_type(lex, 1, i, 1);
	else if (in_put[*i] == '>' && in_put[*i + 1] == '>')
		fill_type(lex, 4, i, 2);
	else if (in_put[*i] == '>' && in_put[*i + 1] != '>')
		fill_type(lex, 2, i, 1);
	else if (in_put[*i] == '&' && in_put[*i + 1] == '&')
		fill_type(lex, 6, i, 2);
	else if (in_put[*i] == '|' && in_put[*i + 1] == '|')
		fill_type(lex, 7, i, 2);
}

t_token	*lex_me(char *in_put)
{
	int		i;
	t_token	*head;
	t_token	*lex;
	t_token	*new_token;

	i = 0;
	head = NULL;
	lex = NULL;
	while (in_put[i])
	{
		new_token = malloc(sizeof(t_token));
		if (new_token == NULL)
			return (NULL);
		new_token->next = NULL;
		new_token->prev = lex;
		if (lex != NULL)
			lex->next = new_token;
		else
			head = new_token;
		lex = new_token;
		init_filling(lex, &i, in_put);
	}
	return (head);
}