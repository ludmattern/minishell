/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 12:48:52 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/04 11:02:28 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

void	skip_spaces(char *arg, int *i)
{
	while (arg[*i] == ' ' || arg[*i] == '	' || arg[*i] == -1)
		(*i)++;
}

char	*replace_substring(char *original, \
	int start, char *replace, t_g_data *data)
{
	int		original_len;
	int		replace_len;
	int		new_len;
	char	*new;
	int		length;

	length = 2;
	original_len = ft_strlen(original);
	replace_len = ft_strlen(replace);
	new_len = original_len - length + replace_len;
	new = malloc(new_len + 1);
	if (!new)
	{
		free(replace);
		free(original);
		fail_exit_shell(data);
	}
	ft_strncpy(new, original, start);
	new[start] = '\0';
	ft_strcat(new, replace);
	ft_strcat(new, original + start + length);
	return (new);
}

char	find_first(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '\'')
			return ('\'');
		else if (arg[i] == '"')
			return ('"');
		i++;
	}
	return ('\0');
}
