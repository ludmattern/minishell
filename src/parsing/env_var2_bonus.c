/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var2_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 09:04:16 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/13 10:24:11 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/bonus/parse_bonus.h"
#include "../../inc/bonus/exec_bonus.h"

void	init_var_ctx(t_var_context *ctx, char *tkn, t_g_data *data)
{
	ctx->i = 0;
	ctx->k = 0;
	ctx->j = 0;
	ctx->s = get_mal_size2(tkn, data);
	if (ctx->s.size == 1)
		ctx->res = NULL;
	else
	{
		ctx->res = malloc(ctx->s.size);
		if (!ctx->res)
			return (free(tkn), fail_exit_shell(data));
		ft_bzero(ctx->res, ctx->s.size);
	}
}

void	quote_in_env_val(char *env_val)
{
	int	i;

	i = 0;
	while (env_val[i])
	{
		if (env_val[i] == '\'')
			env_val[i] = -2;
		else if (env_val[i] == '\"')
			env_val[i] = -3;
		i++;
	}
}

void	toggle_quote_s(char c, bool *dquote, bool *squote)
{
	if (c == '\'' && !*dquote)
		*squote = !*squote;
	if (c == '"' && !*squote)
		*dquote = !*dquote;
}

bool	is_previous_redir(int cursor, char *tkn)
{
	while (cursor > 0 && (tkn[cursor] != '>' && tkn[cursor] != '<'))
		cursor--;
	if (tkn[cursor] == '>' || tkn[cursor] == '<')
		return (true);
	return (false);
}

char	*get_env_var2(char *tkn, t_g_data *data, bool dquote, bool squote)
{
	t_var_context	ctx;

	init_var_ctx(&ctx, tkn, data);
	if (!ctx.res)
		return (NULL);
	while (tkn[ctx.i])
	{
		toggle_quote_s(tkn[ctx.i], &dquote, &squote);
		if (!squote && !dquote && tkn[ctx.i] == '$' && \
		tkn[ctx.i + 1] == '$' && !is_previous_heredoc(ctx.i, tkn))
		{
			ctx.res[ctx.k++] = tkn[ctx.i++];
			ctx.res[ctx.k++] = tkn[ctx.i++];
		}
		else if (!squote && !dquote && tkn[ctx.i] == \
		'$' && (tkn[ctx.i + 1] == '"' || tkn[ctx.i + 1] == '\''))
			ctx.i++;
		else if (tkn[ctx.i] == '$' && tkn[ctx.i + 1] \
		&& !ft_isspace(tkn[ctx.i + 1]) && tkn[ctx.i + 1] \
		!= '$' && !squote && is_previous_heredoc(ctx.i, tkn))
			process_variable(&ctx, tkn, data);
		else
			ctx.res[ctx.k++] = tkn[ctx.i++];
	}
	return (ctx.res[ctx.k] = '\0', ctx.res);
}
