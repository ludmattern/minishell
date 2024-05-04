/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 11:40:23 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/04 13:15:41 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

void	process_lit_quote(char *env_val)
{
	int	i;
	
	i = 0;
	while (env_val[i])
	{
		if (env_val[i] == -2)
			env_val[i] = '\''; 
		else if (env_val[i] == -3)
			env_val[i] = '"';
		i++;
	}
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
		return (NULL);
	output = malloc(ft_strlen(input) + 1);
	if (!output) 
		return (NULL);
	while (input[i] != '\0')
	{
		if (input[i] == -1)
		{
			i++;
			continue ;
		}
		else if (input[i] == '\'')
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
	process_lit_quote(output);
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
			free_lexed(data->lexed);
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
