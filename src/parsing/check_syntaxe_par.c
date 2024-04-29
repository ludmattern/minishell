/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   check_syntaxe_par.c								:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: fprevot <fprevot@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/04/19 15:29:16 by fprevot		   #+#	#+#			 */
/*   Updated: 2024/04/19 15:32:03 by fprevot		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../../inc/parse.h"

void	update_quote_status(char c, char *current_quote, \
int i, const char *cmd)
{
	if ((c == '\'' || c == '\"') && (i == 0 || cmd[i - 1] != '\\'))
	{
		if (*current_quote == 0)
			*current_quote = c;
		else if (*current_quote == c)
			*current_quote = 0;
	}
}

bool	process_parentheses(char c, int *par_count, bool *empty_par)
{
	if (c == '(')
	{
		(*par_count)++;
		*empty_par = true;
	}
	else if (c == ')')
	{
		(*par_count)--;
		if (*empty_par)
			return (false);
	}
	else if (!ft_isspace(c))
		*empty_par = (false);
	return (true);
}

bool	check_par(const char *cmd, int i, int par_count)
{
	char	current_quote;
	bool	empty_par;

	current_quote = 0;
	empty_par = false;
	while (cmd[i])
	{
		update_quote_status(cmd[i], &current_quote, i, cmd);
		if (current_quote == 0)
		{
			if (!process_parentheses(cmd[i], &par_count, &empty_par))
				break ;
			if (par_count < 0)
				break ;
		}
		i++;
	}
	if (empty_par || par_count < 0)
		return (false);
	else if (par_count > 0)
		return (false);
	return (true);
}
