/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 11:40:23 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/05 20:36:25 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

void	process_lit_quote(char *env_val)
{
	int	i;

	i = 0;
	while (env_val[i])
	{
		if (env_val[i] == -2)
			env_val[i] = '\'';
		else if (env_val[i] == -3)
			env_val[i] = '"';
		i++;
	}
}

void	cpy_in_to_out(char in, char **out, char *last_quote, bool *in_quotes)
{
	if (in == -1)
		return ;
	else if (in == '\'' || in == '"')
	{
		if (!(*in_quotes))
		{
			*last_quote = in;
			*in_quotes = true;
		}
		else if (*last_quote == in)
		{
			*last_quote = 0;
			*in_quotes = false;
		}
		else
			*(*out)++ = in;
	}
	else
		*(*out)++ = in;
}

char	*process_quotes(char *input)
{
	int		i;
	char	*tmp;
	char	*output;
	char	last_quote;
	bool	in_quotes;

	in_quotes = false;
	last_quote = 0;
	if (!input)
		return (NULL);
	output = ft_calloc(ft_strlen(input) + 1, sizeof(char));
	if (!output)
		return (NULL);
	tmp = output;
	i = 0;
	while (input[i])
		cpy_in_to_out(input[i++], &tmp, &last_quote, &in_quotes);
	process_lit_quote(output);
	return (output);
}

void	expand_tkn_tab(char **tab, t_g_data *data)
{
	int		j;
	char	*temp;

	j = 0;
	while (tab[j])
	{
		temp = process_quotes(tab[j]);
		if (!temp)
		{
			free_lexed(data->lexed);
			ft_free_double_array(tab);
			fail_exit_shell(data);
		}
		if (temp != tab[j])
			free(tab[j]);
		tab[j++] = temp;
	}
}

char	**expander(char *arg, t_g_data *data)
{
	char	**expanded;

	expanded = NULL;
	expanded = get_tkn_tab(arg, 1, 0, data);
	expand_tkn_tab(expanded, data);
	return (expanded);
}
