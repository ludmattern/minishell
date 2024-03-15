/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 11:40:23 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/14 16:32:41 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

char	*expand_simple_quote(char *tkn)
{
	tkn = skip_quote(tkn);
	return (tkn);
}

char	*expand_double_quote(char *tkn, int last_exit_status)
{
	int		i;
	char	*res;
	char	*status_str;

	tkn = skip_quote(tkn);
	res = tkn;
	i = 0;
	while (res[i])
	{
		if (res[i] == '$')
		{
			tkn = get_env_var(tkn, 0, 0, 0);
			res = tkn;
		}
		else if (res[i] == '$' && res[i + 1] == '?')
		{
			status_str = ft_itoa(last_exit_status);
			tkn = replace_substring(tkn, i, 2, status_str);
			res = tkn;
			free(status_str);
			i += strlen(status_str) - 1;
		}
		i++;
	}
	return (res);
}

char	*expand_without_quote(char *tkn, int last_exit_status, int i)
{
	char	*res;
	char	*status_str;

	res = tkn;
	while (res[i])
	{
		if (res[i] == '$' && res[i + 1] == '?')
		{
			status_str = ft_itoa(last_exit_status);
			tkn = replace_substring(tkn, i, 2, status_str);
			res = tkn;
			free(status_str);
			i += strlen(status_str) - 1;
		}
		else if (res[i] == '$')
		{
			tkn = get_env_var(tkn, 0, 0, 0);
			res = tkn;
		}
		i++;
	}
	return (tkn);
}

void	expand_tkn_tab(char **tab, int last_exit_status)
{
	int	j;

	j = 0;
	while (tab[j])
	{
		if (ft_strchr(tab[j], '"') != NULL)
			tab[j] = expand_double_quote(tab[j], last_exit_status);
		else if (ft_strchr(tab[j], '\'') != NULL)
			tab[j] = expand_simple_quote(tab[j]);
		else
			tab[j] = expand_without_quote(tab[j], last_exit_status, 0);
		j++;
	}
}

char	**expander(char *arg, int last_exit_status)
{
	char	**expanded;

	expanded = malloc(sizeof(char **) * 1);
	expanded = get_tkn_tab(arg, 1, 0, 0);
	expand_tkn_tab(expanded, last_exit_status);
	print_exp(expanded, arg);
	return (expanded);
}


