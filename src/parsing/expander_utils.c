/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 12:48:52 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/14 13:29:49 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

char	*skip_quote(const char *tkn)
{
	char	*res;
	int		i;
	int		j;

	j = 0;
	i = 0;
	res = malloc(ft_strlen(tkn) * sizeof(char));
	if (res == NULL)
		return (NULL);
	if (tkn[i] == '"' || tkn[i] == '\'')
		i++;
	while (tkn[i] != '\0' && tkn[i] != '"' && tkn[i] != '\'')
		res[j++] = tkn[i++];
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
