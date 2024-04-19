/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 12:48:52 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/19 12:55:20 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

void	skip_spaces(char *arg, int *i)
{
	while (arg[*i] == ' ' || arg[*i] == '	' || arg[*i] == -1)
		(*i)++;
}

char	*skip_quote(char *tkn, char q, t_g_data *data)
{
	char	*res;
	int		i;
	int		j;

	i = 0;
	j = 0;
	res = malloc((ft_strlen(tkn) + 1) * sizeof(char));
	if (!res)
		fail_exit_shell(data);
	while (tkn[i] != '\0')
	{
		if (tkn[i] != q && tkn[i] != -1)
			res[j++] = tkn[i];
		i++;
	}
	res[j] = '\0';
	return (res);
}

char	*replace_substring(const char *original, \
	int start, const char *replace, t_g_data *data)
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
		fail_exit_shell(data);
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

void	expand_input(t_g_data *data)
{
	int		j;
	char	*input;
	int		lst;
	char	*temp;

	j = 0;
	input = data->in_put;
	lst = data->last_exit_status;
	while (input[j])
	{
		if (find_first(input) == '"')
			temp = expand_double_quote(input, lst, data);
		else if (find_first(input) == '\'')
			temp = expand_simple_quote(input, data);
		else
			temp = expand_without_quote(input, lst, 0, data);
		if (temp != input)
			free(input);
		input = temp;
		j++;
	}
}
