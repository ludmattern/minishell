/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 13:09:31 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/19 13:16:21 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"
#include "../../inc/exec.h"

void	init_var_ctx(t_var_context *ctx, char *tkn, \
t_g_data *data, bool dquotes)
{
	ctx->i = 0;
	ctx->k = 0;
	ctx->j = 0;
	ctx->s = get_mal_size2(tkn, data->mini_env, data, dquotes);
	if (ctx->s.size == 1)
		ctx->res = NULL;
	else
	{
		ctx->res = malloc(ctx->s.size);
		if (!ctx->res)
			fail_exit_shell(data);
		ft_bzero(ctx->res, ctx->s.size);
	}
}

void	process_variable(t_var_context *ctx, char *tkn, \
t_g_data *data, bool dquotes)
{
	ctx->start = ctx->i;
	while (ft_isalnum(tkn[ctx->i]))
		ctx->i++;
	ctx->env_length = ctx->i - ctx->start;
	ctx->tmp_env = malloc(ctx->env_length + 1);
	if (!ctx->tmp_env)
		fail_exit_shell(data);
	ft_strncpy(ctx->tmp_env, tkn + ctx->start, ctx->env_length);
	ctx->tmp_env[ctx->env_length] = '\0';
	ctx->env_val = ft_get_env3(ctx->tmp_env, data->mini_env, data, dquotes);
	free(ctx->tmp_env);
	if (!ctx->env_val)
	{
		ctx->env_val = malloc(2);
		ctx->env_val[0] = -1;
		ctx->env_val[1] = '\0';
	}
	ctx->j = 0;
	while (ctx->env_val[ctx->j])
		ctx->res[ctx->k++] = ctx->env_val[ctx->j++];
	free(ctx->env_val);
	ctx->env_val = NULL;
}

char	*get_env_var2(char *tkn, t_g_data *data, bool dquotes)
{
	t_var_context	ctx;

	init_var_ctx(&ctx, tkn, data, dquotes);
	if (!ctx.res)
		return (NULL);
	while (tkn[ctx.i])
	{
		if (tkn[ctx.i] == '$' && tkn[ctx.i + 1] \
		&& tkn[ctx.i + 1] != ' ' && tkn[ctx.i + 1] \
		!= '$' && tkn[ctx.i + 1] != '"')
		{
			ctx.i++;
			process_variable(&ctx, tkn, data, dquotes);
		}
		else
			ctx.res[ctx.k++] = tkn[ctx.i++];
	}
	ctx.res[ctx.k] = '\0';
	return (ctx.res);
}
