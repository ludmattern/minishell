/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_env_var.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 14:37:29 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/17 15:07:54 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"
#include "../../inc/exec.h"

void	init_env_ctx_her(t_env_context_her *ctx, t_g_data *data, char *input)
{
	ctx->squotes = false;
	ctx->squotes = false;
	ctx->data = data;
	ctx->res = malloc(sizeof(char *) * 2);
	if (!ctx->res)
		fail_exit_shell(data);
	ctx->res[0] = ft_strdup(input);
	if (!ctx->res)
		fail_exit_shell(data);
	ctx->i = 0;
}

void	process_status_var_h(t_env_context_her *ctx, int *i)
{
	ctx->status_str = ft_itoa(ctx->data->last_exit_status);
	if (!ctx->status_str)
		fail_exit_shell(ctx->data);
	ctx->new = replace_substring(ctx->res[0], *i, ctx->status_str, ctx->data);
	free(ctx->res[0]);
	ctx->res[0] = ctx->new;
	ctx->new = NULL;
	*i += strlen(ctx->status_str) - 1;
	free(ctx->status_str);
}

char	**replace_input_vars(t_g_data *data, char *input, int i)
{
	t_env_context_her	ctx;

	init_env_ctx_her(&ctx, data, input);
	while (ctx.res[0] && ctx.res[0][i] != '\0')
	{
		if (ctx.res[0][i] == '\'')
			ctx.squotes = !ctx.squotes;
		else if (!ctx.squotes && ctx.res[0][i] == '$' && \
		ctx.res[0][i + 1] == '?')
			process_status_var_h(&ctx, &i);
		else if (!ctx.squotes && ctx.res[0][i] == '$' && \
		ctx.res[0][i + 1] == '$')
			ctx.i += 1;
		else if (!ctx.squotes && ctx.res[0][i] == '$' && ctx.res[0][i + 1] \
		!= '\0' && ctx.res[0][i + 1] != ' ' && ctx.res[0][i + 1] != '"')
		{
			ctx.new = get_env_var(ctx.res[0], 0, 0, 0, data);
			free(ctx.res[0]);
			ctx.res[0] = ctx.new;
			ctx.new = NULL;
		}
		i++;
	}
	ctx.res[1] = NULL;
	return (ctx.res);
}
