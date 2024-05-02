/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   expander.c										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: fprevot <fprevot@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/03/14 11:40:23 by fprevot		   #+#	#+#			 */
/*   Updated: 2024/04/29 16:03:03 by fprevot		  ###   ########.fr	   */
/*																			*/
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
	char		*tmp;

	remove_empty_quotes(tkn, 0, 0, false);
	if (is_export != true)
	{
		tmp = skip_quote(tkn, '\'', data);
		tkn = skip_quote(tmp, '"', data);
		free(tmp);
	}
	else
		tkn = skip_quote(tkn, '"', data);
	
	return (tkn);
}

char	*expand_double_quote(char *tkn, int last_exit_status, t_g_data *data)
{
	size_t		i;
	char		*res;
	char		*tmp;

	remove_empty_quotes(tkn, 0, 0, false);
	tmp = skip_quote(tkn, '"', data);
	res = skip_quote(tmp, '\'', data);
	free(tmp);
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

	remove_empty_quotes(tkn, 0, 0, false);
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

void	expand_tkn_tab(char **tab, int last_exit_status, \
t_g_data *data, bool is_export)
{
	int		j;
	char	*temp;

	j = 0;
	while (tab && tab[j])
	{
		if (find_first(tab[j]) == '"')
			temp = expand_double_quote(tab[j], last_exit_status, data);
		else if (find_first(tab[j]) == '\'')
			temp = expand_simple_quote(tab[j], data, is_export);
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
