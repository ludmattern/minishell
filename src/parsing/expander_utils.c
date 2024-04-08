/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 12:48:52 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/08 16:40:49 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

char *skip_quote(char *tkn, char q, t_g_data *data)
{
	char *res;
	int i;
	int j;

	i = 0;
	j = 0;
	res = malloc((ft_strlen(tkn) + 1) * sizeof(char));
	if (!res)
		fail_exit_shell(data);
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
	int start, int length, const char *replace, t_g_data *data)
{
	int		original_len;
	int		replace_len;
	int		new_len;
	char	*new;

	original_len = ft_strlen(original);
	replace_len = ft_strlen(replace);
	new_len = original_len - length + replace_len;
	new = malloc(new_len + 1);
	if (!new)
		fail_exit_shell(data);
	ft_strncpy(new, original, start);
	new[start] = '\0';
	ft_strcat(new, replace);
	ft_strcat(new, original + start + length);
	return (new);
}


