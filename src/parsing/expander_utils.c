/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 12:48:52 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/21 13:25:08 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

char *skip_quote(char *tkn, char q)
{
    char *res;
    int i;
    int j;

    i = 0;
    j = 0;
    res = malloc((ft_strlen(tkn) + 1) * sizeof(char));
    if (res == NULL) 
        return (NULL);
    while (tkn[i] != '\0') 
	{
        if (tkn[i] != q)
            res[j++] = tkn[i];
        i++;
    }
    res[j] = '\0';
    return (res);
}


char	*replace_substring(const char *original, \
	int start, int length, const char *replace)
{
	int		original_len;
	int		replace_len;
	int		new_len;
	char	*new;

	original_len = ft_strlen(original);
	replace_len = ft_strlen(replace);
	new_len = original_len - length + replace_len;
	new = malloc(new_len + 1);
	if (new == NULL)
		return (NULL);
	ft_strncpy(new, original, start);
	new[start] = '\0';
	ft_strcat(new, replace);
	ft_strcat(new, original + start + length);
	return (new);
}

char	**init_tab(int capacity)
{
	char	**tab;

	tab = malloc(capacity * sizeof(char *));
	if (!tab)
		return (NULL);
	return (tab);
}
