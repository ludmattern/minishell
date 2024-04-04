/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:32:26 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/04 18:03:19 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

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

t_envsize get_mal_size(char *tkn, int start, int env_length, int i, t_env *mini_env)
{
	char *env_val;
	t_envsize s = {0};
	s.size = 1; 
		
	while (tkn[i]) 
	{
		if (tkn[i] == '$' && tkn[i + 1] && tkn[i + 1] != ' ' && tkn[i + 1] != '$')
		{
			i++;
			start = i;
			while (tkn[i] && tkn[i] != ' ' && tkn[i] != '/' && tkn[i] != '$') i++;
			env_length = i - start;
			s.env = malloc((env_length > 0 ? env_length : 1) + 1);
			ft_strncpy(s.env, tkn + start, env_length);
			s.env[env_length] = '\0';
			env_val = ft_get_env(s.env, mini_env);
			if (env_val) s.size += ft_strlen(env_val);
			free(s.env);
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

char	*ft_get_env(char *tmp_env, t_env *mini_env)
{
	t_env	*tmp;
	char	*env_val;
	size_t	tmp_env_len;

	tmp = mini_env;
	tmp_env_len = ft_strlen(tmp_env);
	while (tmp)
	{
		if (ft_strncmp(tmp->name, tmp_env, tmp_env_len) == 0)
		{
			env_val = ft_strdup(tmp->value);
			return (env_val);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

char *get_env_var(char *tkn, int i, int k, int j, t_g_data *data)
{
	int start; 
	int env_length;
	char *env_val;

	t_envsize s = get_mal_size(tkn, 0, 0, 0, data->mini_env);
	j++;
	char *res = malloc(s.size);
	if (!res) return NULL;
	ft_bzero(res, s.size);
	while (tkn[i])
	{
		if (tkn[i] == '$' && tkn[i + 1] && tkn[i + 1] != ' ' && tkn[i + 1] != '$')
		{
			i++;  
			start = i;
			while (tkn[i] && tkn[i] != ' ' && tkn[i] != '/' && tkn[i] != '$')
				i++;
			env_length = i - start;
			char *tmp_env;
			if (env_length > 0) 
				tmp_env = malloc(env_length + 1);
			else
				tmp_env = malloc(1 + 1);
			ft_strncpy(tmp_env, tkn + start, env_length);
			tmp_env[env_length] = '\0';
			env_val = ft_get_env(tmp_env, data->mini_env);
			if (env_val)
			{
				while (*env_val) 
					res[k++] = *env_val++;
			}
			free(tmp_env);
		}
		else
			res[k++] = tkn[i++];
	}
	res[k] = '\0';
	return (res);
}

