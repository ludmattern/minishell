/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token_tab_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 11:33:18 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/13 10:24:29 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/bonus/parse_bonus.h"

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
				return (free(tab), NULL);
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

	tab = ft_calloc(size, sizeof(char *));
	if (!tab)
	{
		free_lexed(data->lexed);
		fail_exit_shell(data);
	}
	tab = fill_token_array(arg, tab, &size, i);
	if (!tab)
	{
		ft_free_double_array(tab);
		free_lexed(data->lexed);
		fail_exit_shell(data);
	}
	final_tab = realloc_tab(tab, size, size);
	if (!final_tab)
	{
		ft_free_double_array(tab);
		free_lexed(data->lexed);
		fail_exit_shell(data);
	}
	return (final_tab);
}
