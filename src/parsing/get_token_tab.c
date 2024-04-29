/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token_tab.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 11:33:18 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/29 15:43:32 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

char	*quote_token(char *arg, int *i, t_g_data *data)
{
	int		start;
	char	quote_type;
	char	*token;

	start = *i;
	quote_type = arg[*i];
	(*i)++;
	while (arg[*i] && arg[*i] != quote_type)
		(*i)++;
	token = ft_strndup(arg + start, *i - start + 1);
	if (!token)
		fail_exit_shell(data);
	if (arg[*i])
		(*i)++;
	return (token);
}

char	*space_token(char *arg, int *i)
{
	int		start;
	char	*token;
	bool	dquote;
	bool	squote;

	dquote = false;
	squote = false;
	start = *i;
	while (arg[*i])
	{
		if (arg[*i] == '"')
			dquote = !dquote;
		else if (arg[*i] == '\'')
			squote = !squote;
		if (!squote && !dquote && (ft_isspace(arg[*i]) == true))
			break ;
		(*i)++;
	}
	token = ft_strndup(arg + start, *i - start);
	if (!token)
		return (NULL);
	return (token);
}

char	**realloc_tab(char **tab, int current_size, int new_size)
{
	return (ft_realloc(tab, sizeof(*tab) * \
		current_size, sizeof(*tab) * new_size));
}

char	**fill_token_array(char *arg, char **tab, int *size, int i)
{
	char	**new_tab;
	int		k;

	k = 0;
	while (arg[i])
	{
		skip_spaces(arg, &i);
		if (!arg[i])
			break ;
		if (k >= *size - 1)
		{
			*size += 1;
			new_tab = realloc_tab(tab, k, *size);
			if (!new_tab)
				return (NULL);
			tab = new_tab;
		}
		tab[k++] = space_token(arg, &i);
		if (!tab)
			return (NULL);
	}
	tab[k] = NULL;
	return (tab);
}

char	**get_tkn_tab(char *arg, int size, int i, t_g_data *data)
{
	char	**final_tab;
	char	**tab;

	tab = malloc(size * sizeof(char *));
	if (!tab)
		fail_exit_shell(data);
	tab = fill_token_array(arg, tab, &size, i);
	if (!tab)
		fail_exit_shell(data);
	final_tab = realloc_tab(tab, size, size);
	if (!final_tab)
		fail_exit_shell(data);
	return (final_tab);
}
