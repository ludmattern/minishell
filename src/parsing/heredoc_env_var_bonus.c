/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_env_var_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 14:37:29 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/13 10:24:35 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/bonus/parse_bonus.h"
#include "../../inc/bonus/exec_bonus.h"

void	init_env_ctx_her(t_env_context_her *ctx, t_g_data *data, char *input)
{
	ctx->squotes = false;
	ctx->squotes = false;
	ctx->data = data;
	ctx->res = malloc(sizeof(char *) * 2);
	if (!ctx->res)
	{
		free_io_list(ctx->data->io);
		free(data->her_file);
		free_lexed(data->lexed);
		fail_exit_shell(data);
	}
	(void)input;
	ctx->res[0] = ft_strdup(input);
	if (!ctx->res[0])
	{
		free(ctx->res);
		free(data->her_file);
		free_io_list(ctx->data->io);
		free_lexed(data->lexed);
		fail_exit_shell(data);
	}
	ctx->i = 0;
}

void	process_status_var_h(t_env_context_her *ctx, int *i)
{
	ctx->status_str = ft_itoa(ctx->data->last_exit_status);
	if (!ctx->status_str)
	{
		free(ctx->data->her_file);
		free(ctx->res[0]);
		free(ctx->res);
		free_io_list(ctx->data->io);
		free_lexed(ctx->data->lexed);
		fail_exit_shell(ctx->data);
	}
	ctx->new = replace_substring(ctx->res[0], *i, ctx->status_str, ctx->data);
	free(ctx->res[0]);
	ctx->res[0] = ctx->new;
	ctx->new = NULL;
	*i += ft_strlen(ctx->status_str) - 1;
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
			ctx.new = get_env_var2(ctx.res[0], data, false, false);
			free(ctx.res[0]);
			ctx.res[0] = ctx.new;
			ctx.new = NULL;
		}
		i++;
	}
	ctx.res[1] = NULL;
	return (ctx.res);
}
