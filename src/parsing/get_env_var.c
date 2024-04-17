/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:48:59 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/17 15:09:17 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

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

void	init_env_ctx(t_env_context *ctx, t_g_data *data)
{
	ctx->squotes = false;
	ctx->squotes = false;
	ctx->data = data;
	ctx->res = ft_strdup(data->in_putsave);
	if (!ctx->res)
		fail_exit_shell(data);
	ctx->i = 0;
}

void	update_quote_flags(char c, t_env_context *ctx)
{
	if (c == '"' && ctx->squotes == false)
		ctx->dquotes = !ctx->dquotes;
	else if (c == '\'' && !ctx->dquotes == false)
		ctx->squotes = !ctx->squotes;
}

void	process_status_var(t_env_context *ctx, int *i)
{
	ctx->status_str = ft_itoa(ctx->data->last_exit_status);
	if (!ctx->status_str)
		fail_exit_shell(ctx->data);
	ctx->new = replace_substring(ctx->res, *i, ctx->status_str, ctx->data);
	free(ctx->res);
	ctx->res = ctx->new;
	ctx->new = NULL;
	*i += strlen(ctx->status_str) - 1;
	free(ctx->status_str);
}

char	*replace_env_vars(t_g_data *data, int i)
{
	t_env_context	ctx;

	init_env_ctx(&ctx, data);
	while (ctx.res && ctx.res[i] != '\0')
	{
		update_quote_flags(ctx.res[i], &ctx);
		if (!ctx.squotes)
		{
			if (!ctx.squotes && ctx.res[i] == '$' && ctx.res[i + 1] == '?')
				process_status_var(&ctx, &i);
			else if (!ctx.squotes && ctx.res[i] == '$' && ctx.res[i + 1] == '$')
				i += 1;
			else if (!ctx.squotes && ctx.res[i] == '$' && ctx.res \
			[i + 1] != '\0' && ctx.res[i + 1] != ' ' && ctx.res[i + 1] != '"')
			{
				ctx.new = get_env_var2(ctx.res, 0, 0, 0, data, ctx.dquotes);
				if (ctx.new == NULL)
					return (free(ctx.res), strdup(""));
				free(ctx.res);
				ctx.res = ctx.new;
				ctx.new = NULL;
				i--;
			}
		}
		i++;
	}
	return (ctx.res);
}
