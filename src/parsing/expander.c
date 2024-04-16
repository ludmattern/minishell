/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 11:40:23 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/16 11:19:12 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

char	*expand_simple_quote(char *tkn, t_g_data *data)
{
	tkn = skip_quote(tkn, '\'', data);
	return (tkn);
}

char	*expand_double_quote(char *tkn, int last_exit_status, t_g_data *data)
{
	size_t		i;
	char	*res;
	

	res = skip_quote(tkn, '"', data);
	i = 0;
	(void)last_exit_status;
	while (res && res[i] != '\0')
	{
		i++;
		if (i >= ft_strlen(res))  
			break;
	}
	return (res);
}

char *expand_without_quote(char *tkn, int last_exit_status, size_t i, t_g_data *data)
{
	char *res = tkn;
	(void)last_exit_status;
	(void)data;
	while (res && res[i] != '\0')
	{
		i++;
		if (i >= ft_strlen(res))  
			break;
	}
	return (res);
}

char find_first(char *arg) 
{
	int i = 0;
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

void expand_input(t_g_data *data)
{
	int j = 0;
	char *input = data->in_put;
	int lst = data->last_exit_status;
	char *temp;
	
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
void expand_tkn_tab(char **tab, int last_exit_status, t_g_data *data)
{
	int j = 0;
	char *temp;

	while (tab && tab[j])
	{
		if (find_first(tab[j]) == '"')
			temp = expand_double_quote(tab[j], last_exit_status, data);
		else if (find_first(tab[j]) == '\'') 
			temp = expand_simple_quote(tab[j], data);
		else
			temp = expand_without_quote(tab[j], last_exit_status, 0, data);
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
	//print_exp(expanded, arg);
	expand_tkn_tab(expanded, last_exit_status, data);
    
	return (expanded);
}

/*
char	*expand_simple_quote(char *tkn, t_g_data *data)
{
	tkn = skip_quote(tkn, '\'', data);
	return (tkn);
}

char	*expand_double_quote(char *tkn, int last_exit_status, t_g_data *data)
{
	size_t		i;
	char	*res;
	char	*status_str;
	char *new;

	res = skip_quote(tkn, '"', data);
	i = 0;
	while (res && res[i] != '\0')
	{
		if (res[i] == '$' && res[i + 1] == '?')
		{
			status_str = ft_itoa(last_exit_status);
			if (!status_str)
				fail_exit_shell(data);
			new = replace_substring(res, i, 2, status_str, data);
			free(res);
			res = new;
			i += ft_strlen(status_str);
			free(status_str);  
		}
		else if (res[i] == '$' && res[i + 1] == '$')
			i += 2;
		else if (res[i] == '$' && res[i + 1] != '\0' && res[i + 1] != ' ')
		{
			new = get_env_var(res, 0,0,0,data);
			free(res);
			res = new;
		}
		else
			i++;
		if (i >= ft_strlen(res))  
			break;
	}
	return (res);
}

char *expand_without_quote(char *tkn, int last_exit_status, size_t i, t_g_data *data)
{
	char *res = tkn;
	char *status_str;
	int len;

	while (res && res[i] != '\0')
	{
		if (res[i] == '$' && res[i + 1] == '?')
		{
			status_str = ft_itoa(last_exit_status);
			if (!status_str)
				fail_exit_shell(data);
			tkn = replace_substring(tkn, i, 2, status_str, data);
			len = ft_strlen(status_str);
			i += len;
			free(status_str);
			res = tkn;  
		}
		else if (res[i] == '$' && res[i + 1] == '$')
		{
			i += 2;
		}
		else if (res[i] == '$' && res[i + 1] != '\0' && res[i + 1] != ' ')
		{
			tkn = get_env_var(tkn, 0, 0, 0, data);
			res = tkn; 
		}
		else
			i++;
		if (i >= ft_strlen(res))  
			break;
	}
	return (res);
}*/