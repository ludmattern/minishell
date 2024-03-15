/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token_tab.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 16:58:58 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/15 11:09:27 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

char	*quote_token(char *arg, int *i)
{
	int		start;
	char	quote_type;
	char	*token;

	start = *i + 1;
	quote_type = arg[*i];
	(*i)++;
	while (arg[*i] && arg[*i] != quote_type)
		(*i)++;
	token = ft_strndup(arg + start, *i - start);
	if (arg[*i])
		(*i)++;
	return (token);
}

char	*space_token(char *arg, int *i)
{
	int		start;
	char	*token;

	start = *i;
	while (arg[*i] && arg[*i] != ' ')
		(*i)++;
	token = ft_strndup(arg + start, *i - start);
	return (token);
}

void	skip_spaces(char *arg, int *i)
{
	while (arg[*i] == ' ')
		(*i)++;
}

char	**realloc_tab(char **tab, int new_size, int j)
{
	char	**new_tab;

	new_tab = my_realloc(tab, new_size * sizeof(char *));
	if (!new_tab)
	{
		while (tab[j])
		{
			free(tab[j]);
			j++;
		}
		free(tab);
	}
	return (new_tab);
}

char	**get_tkn_tab(char *arg, int size, int i, int k)
{
	char	**tab;
	char	**new_tab;

	tab = malloc(size * sizeof(char *));
	if (!tab)
		return (NULL);
	while (arg[i])
	{
		skip_spaces(arg, &i);
		if (!arg[i])
			break ;
		if (arg[i] == '"' || arg[i] == '\'')
			tab[k++] = quote_token(arg, &i);
		else
			tab[k++] = space_token(arg, &i);
		if (k >= size)
		{
			size += 1;
			new_tab = realloc_tab(tab, size, 0);
			if (!new_tab)
				return (NULL);
			tab = new_tab;
		}
	}
	return (tab[k] = NULL, tab);
}
