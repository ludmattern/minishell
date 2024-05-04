/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   env_var.c										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: fprevot <fprevot@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/04/17 16:30:08 by fprevot		   #+#	#+#			 */
/*   Updated: 2024/04/19 14:34:21 by fprevot		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../../inc/parse.h"
#include "../../inc/exec.h"

void	smoremore(t_envsize *s, int *i, char *env_val, int c)
{
	if (c == 1)
	{
		s->size++;
		(*i)++;
	}
	else if (c == 2)
	{
		if (env_val != NULL)
			s->envsize = ft_strlen(env_val);
		else
			s->envsize = 0;
	}
}

char	*ft_get_env(char *tmp_env, t_env *mini_env)
{
	t_env	*tmp;
	char	*env_val;
	size_t	tmp_env_len;

	tmp = mini_env;
	tmp_env_len = ft_strlen(tmp_env);
	while (tmp)
	{
		if (ft_strncmp(tmp->name, tmp_env, tmp_env_len + 1) == 0)
		{
			env_val = ft_strdup(tmp->value);
			return (env_val);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

const char	*skip_s(const char *s)
{
	while (*s == ' ')
		s++;
	return (s - 1);
}

char	*ft_env_strdup(const char *s, bool dquotes \
, size_t	length, size_t	index)
{
	char	*str;
	char	*original;

	while (s && s[index])
	{
		if (s[index] == ' ' && !dquotes)
		{
			while (s[index] && s[index] == ' ')
				index++;
			index--;
		}
		length++;
		index++;
	}
	str = ft_calloc((length + 1), sizeof(char));
	if (str == NULL)
		return (NULL);
	original = str;
	while (s && *s)
	{
		if (*s == ' ' && !dquotes)
			s = skip_s(s);
		*str++ = *s++;
	}
	return (original);
}

char	*ft_get_env3(char *tmp_env, t_env *mini_env, t_g_data *data)
{
	t_env	*current;
	char	*env_val;
	size_t	tmp_env_len;

	current = mini_env;
	tmp_env_len = ft_strlen(tmp_env);
	while (current)
	{
		if (ft_strncmp(current->name, tmp_env, tmp_env_len + 1) == 0)
		{
			if (!current->value || !current->value[0])
				return (NULL);
			env_val = ft_strdup(current->value);
			if (!env_val)
			{
				free(data->in_putsave);
				free_mini_env(mini_env);
				free(data->data);
				free(data);
				exit(EXIT_FAILURE);
			}
			return (env_val);
		}
		current = current->next;
	}
	return (NULL);
}
