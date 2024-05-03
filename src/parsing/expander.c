/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 11:40:23 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/03 11:21:01 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

void	remove_empty_quotes(char *str, int rp, int wp, bool in_squote)
{
	bool	in_dquote;

	in_dquote = false;
	while (str[rp] != '\0')
	{
		if (str[rp] == '\'' && !in_dquote)
		{
			if (!in_squote && str[rp + 1] == '\'')
			{
				rp += 2;
				continue ;
			}
			in_squote = !in_squote;
		}
		else if (str[rp] == '"' && !in_squote)
		{
			if ((!in_dquote && str[rp + 1] == '"'))
			{
				rp += 2;
				continue ;
			}
			in_dquote = !in_dquote;
		}
		else if (str[rp] == -1)
			rp += 1;
		str[wp++] = str[rp++];
	}
	str[wp] = '\0';
}

char	*expand_simple_quote(char *tkn, t_g_data *data, bool is_export)
{
	//char		*tmp;
	remove_empty_quotes(tkn, 0, 0, false);
	if (is_export != true)
	{
		tkn = skip_quote(tkn, '\'', data);
		//tkn = skip_quote(tmp, '"', data);
		//free(tmp);
	}
	else
		tkn = skip_quote(tkn, '"', data);
	return (tkn);
}

char	*expand_double_quote(char *tkn, int last_exit_status, t_g_data *data)
{
	size_t		i;
	char		*res;
	//char		*tmp;

	remove_empty_quotes(tkn, 0, 0, false);
	res = skip_quote(tkn, '"', data);
	//res = skip_quote(tmp, '\'', data);
	//free(tmp);
	if (!tkn)
		return (NULL);
	i = 0;
	(void)last_exit_status;
	while (res && res[i] != '\0')
	{
		if (i++ >= ft_strlen(res))
			break ;
	}
	return (res);
}

char	*expand_without_quote(char *tkn, int 
last_exit_status, size_t i, t_g_data *data)
{
	char	*res;

	remove_empty_quotes(tkn, 0, 0, false);
	res = tkn;
	(void)last_exit_status;
	(void)data;
	while (res && res[++i] != '\0')
	{
		if (i >= ft_strlen(res))
			break ;
	}
	return (res);
}

char *process_quotes(char *input, t_g_data *data)
{
	char	*output;
	bool	in_single = false;
	bool	in_double = false;
	int		i = 0;
	int		j = 0;
	(void)data;
	if (!input) 
		return NULL;

    output = malloc(strlen(input) + 1);
    if (!output) return NULL;

    while (input[i] != '\0')
	{
		if (input[i] == -1)
			i++;
        if (input[i] == '\'')
		{
            if (!in_double) 
				in_single = !in_single;
            else 
				output[j++] = input[i];
        }
        else if (input[i] == '"')
		{
            if (!in_single) 
				in_double = !in_double;
            else 
				output[j++] = input[i];
        }
        else
		{
            if (in_single || in_double || (!in_single && !in_double))
                output[j++] = input[i];
        }
        i++;
    }
    output[j] = '\0';
    return (output);
}

void	expand_tkn_tab(char **tab, int last_exit_status, \
t_g_data *data, bool is_export)
{
	int		j;
	char	*temp;

	(void)last_exit_status;
	(void)is_export;
	j = 0;
	while (tab[j])
	{
		temp =  process_quotes(tab[j], data);
		
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

char	**expander(char *arg, int last_exit_status, \
t_g_data *data, bool is_export)
{
	char	**expanded;

	expanded = NULL;
	(void)last_exit_status;
	expanded = get_tkn_tab(arg, 1, 0, data);
	expand_tkn_tab(expanded, last_exit_status, data, is_export);
	return (expanded);
}
