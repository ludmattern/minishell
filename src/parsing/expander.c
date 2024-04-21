/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 11:40:23 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/21 17:11:42 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

char	*expand_simple_quote(char *tkn, t_g_data *data)
{
	tkn = skip_quote(tkn, '\'', data);
	if (!tkn)
		return (NULL);
	return (tkn);
}

char	*expand_double_quote(char *tkn, int last_exit_status, t_g_data *data)
{
	size_t		i;
	char		*res;

	res = skip_quote(tkn, '"', data);
	if (!tkn)
		return (NULL);
	i = 0;
	(void)last_exit_status;
	while (res && res[i] != '\0')
	{
		i++;
		if (i >= ft_strlen(res))
			break ;
	}
	return (res);
}

char	*expand_without_quote(char *tkn, int \
last_exit_status, size_t i, t_g_data *data)
{
	char	*res;

	res = tkn;
	(void)last_exit_status;
	(void)data;
	while (res && res[i] != '\0')
	{
		i++;
		if (i >= ft_strlen(res))
			break ;
	}
	return (res);
}

void	expand_tkn_tab(char **tab, int last_exit_status, t_g_data *data)
{
	int		j;
	char	*temp;

	j = 0;
	while (tab && tab[j])
	{
		if (find_first(tab[j]) == '"')
			temp = expand_double_quote(tab[j], last_exit_status, data);
		else if (find_first(tab[j]) == '\'')
			temp = expand_simple_quote(tab[j], data);
		else
			temp = expand_without_quote(tab[j], last_exit_status, 0, data);
		if (!temp)
		{
			ft_free_double_array(tab);
			fail_exit_shell(data);
		}
		if (temp != tab[j])
			free(tab[j]);
		tab[j] = temp;
		j++;
	}
}

char	**expander(char *arg, int last_exit_status, t_g_data *data)
{
	char	**expanded;

	expanded = NULL;
	(void)last_exit_status;
	expanded = get_tkn_tab(arg, 1, 0, data);
	expand_tkn_tab(expanded, last_exit_status, data);
	return (expanded);
}
