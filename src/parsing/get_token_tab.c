/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token_tab.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 16:58:58 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/20 14:31:54 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

char	*quote_token(char *arg, int *i)
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
	if (arg[*i])
		(*i)++;
	return (token);
}

char	*space_token(char *arg, int *i)
{
	int		start;
	char	*token;
    bool    dquote = false;
    bool    squote = false;
    

	start = *i;
    while (arg[*i])
    {
        if (arg[*i] == '"')
            dquote = !dquote;
        else if (arg[*i] == '\'')
            squote = !squote;
        if (!squote && !dquote && arg[*i] == ' ')
            break;
        (*i)++;
    }
	token = ft_strndup(arg + start, *i - start);
	return (token);
}

void	skip_spaces(char *arg, int *i)
{
	while (arg[*i] == ' ')
		(*i)++;
}


char	**realloc_tab(char **tab, int current_size, int new_size) 
{
    return (my_realloc(tab, sizeof(*tab) * current_size, sizeof(*tab) * new_size));
}

char	**get_tkn_tab(char *arg, int size, int i, int k) 
{
    char	**tab;
	char	**final_tab;

    

	tab = malloc(size * sizeof(char *));
    if (!tab)
        return (NULL);
    while (arg[i]) 
	{
        skip_spaces(arg, &i);
        if (!arg[i])
            break;
        if (k >= size - 1) 
		{ 
            size += 1;
            char **new_tab = realloc_tab(tab, k, size);
            if (!new_tab)
                return (NULL);
            tab = new_tab;
        }
        tab[k++] = space_token(arg, &i);
    }
    final_tab = realloc_tab(tab, k, k + 1);
    if (!final_tab)
        return (NULL);
    final_tab[k] = NULL; 
    
    return (final_tab);
}


