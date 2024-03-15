/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 10:32:24 by fprevot           #+#    #+#             */
/*   Updated: 2024/03/15 12:44:08 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

void	smoremore(t_envsize *s, int *i, char *env_val, int c)
{
	if (c == 1)
	{
		s->size++;
		(*i)++;
	}
	else if (c == 2)
	{
		if (!env_val)
			s->envsize = 0;
		else
			s->envsize = ft_strlen(env_val);
	}
}

t_envsize	get_mal_size(char *tkn, int start, int env_length, int i)
{
	t_envsize	s;

	s.envsize = 0;
	s.size = 0;
	while (tkn[i])
	{
		if (tkn[i] == '$' && tkn[i + 1] != '\0' && tkn[i + 1] != ' ')
		{
			i++;
			start = i;
			while (tkn[i] != ' ' && tkn[i] != '\0' && tkn[i] != '/')
				i++;
			env_length = i - start;
			s.env = malloc(env_length + 1);
			ft_strncpy(s.env, tkn + start, env_length);
			s.env[env_length] = '\0';
			s.env_val = getenv(s.env);
			if (s.env_val)
				s.size += ft_strlen(s.env_val);
		}
		else
			smoremore(&s, &i, s.env_val, 1);
	}
	s.size = s.size + 1;
	smoremore(&s, &i, s.env_val, 2);
	return (free(s.env), s.env = NULL, s);
}

char	*get_env_var(char *tkn, int i, int k, int j)
{
	t_envsize	s;
	char		*env_val;

	s = get_mal_size(tkn, 0, 0, 0);
	s.res = malloc(s.size);
	ft_bzero(s.res, s.size);
	s.env = malloc(s.envsize);
	ft_bzero(s.env, s.envsize);
	env_val = NULL;
	while (tkn[i])
	{
		if (tkn[i] == '$' && (tkn[i + 1] != '\0' && tkn[i + 1] != ' '))
		{
			i++;
			while (tkn[i] != ' ' && tkn[i] != '\0' && tkn[i] != '/')
				s.env[j++] = tkn[i++];
			env_val = getenv(s.env);
			if (env_val)
			{
				while (*env_val)
					s.res[k++] = *env_val++;
			}
		}
		else
			s.res[k++] = tkn[i++];
	}
	s.res[k] = '\0';
	return (free(s.env), s.res);
}
