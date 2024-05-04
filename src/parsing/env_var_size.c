/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_size.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 13:37:08 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/04 10:45:53 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"
#include "../../inc/exec.h"

void	handle_failure(char *tkn, char *env, t_g_data *data)
{
	if (tkn)
		free(tkn);
	if (env)
		free(env);
	free(data->in_putsave);
	free_mini_env(data->mini_env);
	free(data->data);
	free(data);
	printf("Memory fail\n");
	exit(EXIT_FAILURE);
}

void	init_var_size_ctx(t_var_size_context *s)
{
	s->i = 0;
	s->s.env = NULL;
	s->s.env_val = NULL;
	s->s.envsize = 0;
	s->s.res = NULL;
	s->s.size = 1;
	s->env_val = NULL;
	s->env_length = 0;
}

void	handle_variable_size(t_var_size_context *s, \
char *tkn, t_g_data *data, bool dquotes)
{
	char	*env;
	char	*env_val;

	s->start = s->i;
	while (ft_isalnum(tkn[s->i]))
		s->i++;
	s->env_length = s->i - s->start;
	env = malloc(s->env_length + 1);
	if (!env)
		handle_failure(tkn, env, data);
	ft_strncpy(env, tkn + s->start, s->env_length);
	env[s->env_length] = '\0';
	env_val = ft_get_env3(env, data->mini_env, data, dquotes);
	if (!env_val)
	{
		env_val = malloc(2);
		if (!env_val)
			handle_failure(tkn, env, data);
		env_val[0] = -1;
		env_val[1] = '\0';
	}
	if (env_val)
		s->s.size += ft_strlen(env_val);
	free(env);
	free(env_val);
}

t_envsize	get_mal_size2(char *tkn, t_env *mini_env, t_g_data \
*data, bool dquotes)
{
	t_var_size_context	s;

	init_var_size_ctx(&s);
	(void)mini_env;
	while (tkn[s.i])
	{
		if (tkn[s.i] == '$' && tkn[s.i + 1] && tkn[s.i + 1] \
		!= ' ' && tkn[s.i + 1] != '$' && tkn[s.i + 1] != '"')
		{
			s.i++;
			handle_variable_size(&s, tkn, data, dquotes);
		}
		else
		{
			s.s.size++;
			s.i++;
		}
	}
	return (s.s);
}
