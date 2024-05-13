/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 09:06:06 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/13 10:24:47 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/bonus/parse_bonus.h"

int	fill_value(t_token *lex, int num)
{
	if (num == 5)
		lex->value = ft_strdup("|\0");
	else if (num == 6)
		lex->value = ft_strdup("&&\0");
	else if (num == 7)
		lex->value = ft_strdup("||\0");
	else if (num == 8)
		lex->value = ft_strdup("\n\0");
	else if (num == 9)
		lex->value = ft_strdup("(\0");
	else if (num == 10)
		lex->value = ft_strdup(")\0");
	else if (num == 11)
		lex->value = ft_strdup("&\0");
	if (!lex->value)
		return (-1);
	return (0);
}

int	fill_type(t_token *lex, int num, int *i, int size)
{
	imore(size, i);
	if (num == 5)
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
	else if (num == 11)
		lex->type = 11;
	if (fill_value(lex, num) == -1)
		return (-1);
	return (0);
}

int	fill_t_word(t_token *lex, int *i, int j, char *in_put)
{
	int		start;
	int		end;
	int		wl;

	skip_space(in_put, i);
	start = *i;
	while (in_put[*i] != '|' && !(in_put[*i] == '&' \
		&& in_put[*i + 1] == '&') && in_put[*i] != '('\
		&& in_put[*i] != ')' && in_put[*i] != '\0')
	{
		if (in_put[*i] == '"' || in_put[*i] == '\'')
			imoremore_quote(in_put, i, in_put[*i]);
		if (in_put[*i] != ' ')
			end = *i;
		(*i)++;
	}
	wl = end - start + 1;
	lex->value = malloc(wl + 1);
	if (!lex->value)
		return (-1);
	while (++j < wl)
		lex->value[j] = in_put[start + j];
	lex->value[wl] = '\0';
	lex->type = T_WORD;
	return (0);
}

int	init_filling(t_token *lex, int *i, char *in_put, int t)
{
	while (in_put && ft_isspace(in_put[*i]))
		(*i)++;
	if (in_put[*i] == '(')
		t = fill_type(lex, 9, i, 1);
	else if (in_put[*i] == ')')
		t = fill_type(lex, 10, i, 1);
	else if (in_put[*i] != '|' && \
		!(in_put[*i] == '&' && in_put[*i + 1] == '&') && in_put[*i] != '(' \
		&& in_put[*i] != ')' && in_put[*i] != '\0')
	{
		if (fill_t_word(lex, i, -1, in_put) == -1)
			return (-1);
	}
	else if (in_put[*i] == '|' && in_put[*i + 1] != '|')
		t = fill_type(lex, 5, i, 1);
	else if (in_put[*i] == '&' && in_put[*i + 1] != '&')
		t = fill_type(lex, 11, i, 1);
	else if (in_put[*i] == '&' && in_put[*i + 1] == '&')
		t = fill_type(lex, 6, i, 2);
	else if (in_put[*i] == '|' && in_put[*i + 1] == '|')
		t = fill_type(lex, 7, i, 2);
	if (t == -1)
		return (-1);
	return (0);
}

t_token	*lex_me(char *in_put, int i, t_g_data *g_data)
{
	t_token	*head;
	t_token	*lex;
	t_token	*new_token;

	new_token = NULL;
	head = NULL;
	lex = NULL;
	while (in_put && in_put[i])
	{
		if (ft_isspace(in_put[i]))
			if (is_end(in_put, i))
				break ;
		new_token = calloc(1, sizeof(t_token));
		if (!new_token)
			return (g_data->exit_fail = -1, head);
		new_token->prev = lex;
		if (lex != NULL)
			lex->next = new_token;
		else
			head = new_token;
		lex = new_token;
		if (init_filling(lex, &i, in_put, 1) == -1)
			return (g_data->exit_fail = -1, head);
	}
	return (head);
}
