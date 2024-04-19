/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:30:08 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/19 12:32:46 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"
#include "../../inc/exec.h"

void smoremore(t_envsize *s, int *i, char *env_val, int c)
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


char	*ft_env_strdup(const char *s, bool dquotes)
{
	char	*str;
	char	*original;
	size_t	length;
	size_t	index;

	length = 0;
	index = 0;
	while (s[index])
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
	while (*s)
	{
		if (*s == ' ' && !dquotes)
		{
			while (*s && *s == ' ')
				s++;
			s--;
		}
		*str++ = *s++;
	}
	return (original);
}

char	*ft_get_env3(char *tmp_env, t_env *mini_env, t_g_data *data, bool dquotes)
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
			env_val = ft_env_strdup(tmp->value, dquotes);
			if (!env_val)
				fail_exit_shell(data);
			return (env_val);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

t_envsize get_mal_size2(char *tkn, int start, int env_length, int i, t_env *mini_env, t_g_data *data, bool dquotes)
{
	char *env_val;
	t_envsize s = {0};
	s.size = 1; 

	while (tkn[i]) 
	{
		if (tkn[i] == '$' && tkn[i + 1] && tkn[i + 1] != ' ' && tkn[i + 1] != '$' && tkn[i + 1] != '"')
		{
			i++;
			start = i;
			while (ft_isalnum(tkn[i])) 
				i++;
			env_length = i - start;
			if (env_length > 0)
				s.env = malloc(env_length + 1);
			else
				s.env = malloc(1);
			if (!s.env)
				fail_exit_shell(data);
			ft_strncpy(s.env, tkn + start, env_length);
			s.env[env_length] = '\0';
			env_val = ft_get_env3(s.env, mini_env, data, dquotes);
			if (!env_val)
			{
				env_val = malloc(2 * sizeof(char));
				env_val[0] = -1;
				env_val[1] = '\0';
			}
			if (env_val) 
				s.size += ft_strlen(env_val);
			free(s.env);
			free(env_val);
			s.env = NULL;
		}
		else
		{
			s.size++;
			i++;
		}
	}
	return (s);  
}
