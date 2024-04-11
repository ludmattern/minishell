/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:32:26 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/11 17:05:40 by fprevot          ###   ########.fr       */
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

char	*ft_get_env2(char *tmp_env, t_env *mini_env, t_g_data *data)
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
			if (!env_val)
				fail_exit_shell(data);
			return (env_val);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

t_envsize get_mal_size(char *tkn, int start, int env_length, int i, t_env *mini_env, t_g_data *data)
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
			while (tkn[i] && tkn[i] != ' ' && tkn[i] != '/' && tkn[i] != '$' && tkn[i] != '"') 
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
			env_val = ft_get_env2(s.env, mini_env, data);
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


char *get_env_var(char *tkn, int i, int k, int j, t_g_data *data) 
{
    int start;
	char *res;
    int env_length;
    char *env_val = NULL;
	t_envsize s;
	memset(&s, 0, sizeof(t_envsize));
	
	s = get_mal_size(tkn, 0, 0, 0, data->mini_env, data);
    res = malloc(s.size);
    if (!res)
        fail_exit_shell(data);
    ft_bzero(res, s.size);

    while (tkn[i]) 
	{
        if (tkn[i] == '$' && tkn[i + 1] && tkn[i + 1] != ' ' && tkn[i + 1] != '$' && tkn[i + 1] != '"')
		{
            i++;
			j = 0;
            start = i;
            while (tkn[i] && tkn[i] != ' ' && tkn[i] != '/' && tkn[i] != '$' && tkn[i] != '"')
                i++;
            env_length = i - start;
            char *tmp_env = malloc(env_length + 1);
            if (!tmp_env)
                fail_exit_shell(data);
            ft_strncpy(tmp_env, tkn + start, env_length);
            tmp_env[env_length] = '\0';
            env_val = ft_get_env2(tmp_env, data->mini_env, data);
            free(tmp_env);
            if (env_val) 
			{
                while (env_val[j])
                    res[k++] = env_val[j++];
                free(env_val);
                env_val = NULL; 
            }
        } 
		else 
            res[k++] = tkn[i++];
    }
    res[k] = '\0';
    return (res);
}


char *replace_env_vars(t_g_data *data)
{
	char *res;
    size_t i = 0;
    bool squotes = false;
    res = ft_strdup(data->in_putsave);
	if (!res)
		fail_exit_shell(data);
    char *status_str;
	char *new;


    while (res && res[i] != '\0') 
	{
        if (res[i] == '\'')
		{
            squotes = !squotes;
        } 
		else if (!squotes && res[i] == '$' && res[i + 1] == '?')
		{
            status_str = ft_itoa(data->last_exit_status);
            if (!status_str) fail_exit_shell(data); 
            new = replace_substring(res, i, 2, status_str, data);
            free(res);
            res = new;
			new = NULL;
            i += ft_strlen(status_str) - 1;
            free(status_str);
        }
		else if (!squotes && res[i] == '$' && res[i + 1] == '$')
            i += 1;
		else if (!squotes && res[i] == '$' && res[i + 1] != '\0' && res[i + 1] != ' ' && res[i + 1] != '"')
		{
            new = get_env_var(res, 0,0, 0, data);
            free(res);
            res = new;
			new = NULL;
        }
        i++;
    }
    return (res);
}


/*
char *get_env_var(char *tkn, int i, int k, int j, t_g_data *data)
{
	int start; 
	int env_length;
	char *env_val = NULL;

	t_envsize s = get_mal_size(tkn, 0, 0, 0, data->mini_env, data);
	j++;
	char *res = malloc(s.size);
	if (!res)
		fail_exit_shell(data);
	ft_bzero(res, s.size);
	while (tkn[i])
	{
		if (tkn[i] == '$' && tkn[i + 1] && tkn[i + 1] != ' ' && tkn[i + 1] != '$' && tkn[i + 1] != '"')
		{
			i++;  
			start = i;
			while (tkn[i] && tkn[i] != ' ' && tkn[i] != '/' && tkn[i] != '$' && tkn[i] != '"')
				i++;
			env_length = i - start;
			char *tmp_env;
			if (env_length > 0) 
				tmp_env = malloc(env_length + 1);
			else
				tmp_env = malloc(1 + 1);
			if (!tmp_env)
				fail_exit_shell(data);
			ft_strncpy(tmp_env, tkn + start, env_length);
			tmp_env[env_length] = '\0';
			env_val = ft_get_env2(tmp_env, data->mini_env, data);
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
*/


