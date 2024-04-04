/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:32:26 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/04 11:42:33 by fprevot          ###   ########.fr       */
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

t_envsize get_mal_size(char *tkn, int start, int env_length, int i)
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
			env_val = getenv(s.env);
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


char *get_env_var(char *tkn, int i, int k, int j, t_global_data *data)
{
	int start; 
	int env_length;
	char *env_val;

	data->local_env = NULL;
	t_envsize s = get_mal_size(tkn, 0, 0, 0);
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
			env_val = getenv(tmp_env);
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

