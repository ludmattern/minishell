/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   env_var2.c										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: fprevot <fprevot@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/04/19 13:09:31 by fprevot		   #+#	#+#			 */
/*   Updated: 2024/05/03 15:21:26 by fprevot		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../../inc/parse.h"
#include "../../inc/exec.h"

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

bool	is_previous_redir(int cursor, char *tkn)
{
	while (cursor > 0 && (tkn[cursor] != '>' && tkn[cursor] != '<'))
		cursor--;
	if (tkn[cursor] == '>' || tkn[cursor] == '<')
		return (true);
	return (false);
}

int	ft_count_wrds(char const *s)
{
	int	nb_wrd;
	int		in_wrd;

	nb_wrd = 0;
	in_wrd = 0;
	if (!s)
		return (0);
	while (*s)
	{
		if (!ft_isspace(*s) && !in_wrd)
		{
			in_wrd = 1;
			nb_wrd++;
		}
		else if (ft_isspace(*s) && in_wrd)
			in_wrd = 0;
		s++;
	}
	return (nb_wrd);
}

void	process_variable(t_var_context *ctx, char *tkn, t_g_data *data)
{
	bool	previous_redir;
	ctx->start = ctx->i;
	previous_redir = is_previous_redir(ctx->start, tkn);
	while (ft_isalpha(tkn[ctx->i]))
		ctx->i++;
	if ((tkn[ctx->i] >= '0' && tkn[ctx->i] <= '9') || tkn[ctx->i] == '_')
		ctx->i++;
	ctx->env_length = ctx->i - ctx->start;
	ctx->tmp_env = calloc(ctx->env_length + 1, sizeof(char));
	if (!ctx->tmp_env)
		return (free(tkn), free(ctx->res), fail_exit_shell(data));
	ft_strncpy(ctx->tmp_env, tkn + ctx->start, ctx->env_length);
	ctx->env_val = ft_get_env3(ctx->tmp_env, data->mini_env, data, tkn);
	free(ctx->tmp_env);
	if (previous_redir && ft_count_wrds(ctx->env_val) > 1)
	{
		ft_free(ctx->env_val);
		ctx->env_val = calloc(2, sizeof(char));
		if (!ctx->env_val)
			return (free(tkn), free(ctx->res), fail_exit_shell(data));
		ctx->env_val[0] = -4;
	}
	if (!ctx->env_val)
	{
		ctx->env_val = calloc(2, sizeof(char));
		if (!ctx->env_val)
			return (free(tkn), free(ctx->res), fail_exit_shell(data));
		ctx->env_val[0] = -1;
	}
	quote_in_env_val(ctx->env_val);
	ctx->j = 0;
	while (ctx->env_val[ctx->j])
		ctx->res[ctx->k++] = ctx->env_val[ctx->j++];
	free(ctx->env_val);
	ctx->env_val = NULL;
}

char	*get_env_var2(char *tkn, t_g_data *data, bool dquote, bool squote)
{
	t_var_context	ctx;

	init_var_ctx(&ctx, tkn, data);
	if (!ctx.res)
		return (NULL);
	while (tkn[ctx.i])
	{
		if (tkn[ctx.i] == '\'' && !dquote)
			squote = !squote;
		if (tkn[ctx.i] == '"' && !squote)
			dquote = !dquote;
		if (!squote && !dquote && tkn[ctx.i] == '$' && tkn[ctx.i + 1] == '$' && !is_previous_heredoc(ctx.i, tkn))
		{
			ctx.res[ctx.k++] = tkn[ctx.i++];
			ctx.res[ctx.k++] = tkn[ctx.i++];
		}
		else if (!squote && !dquote && tkn[ctx.i] == '$' && (tkn[ctx.i + 1] == '"' ||  tkn[ctx.i + 1] == '\''))
			ctx.i++;
		else if (tkn[ctx.i] == '$' && tkn[ctx.i + 1] \
		&& !ft_isspace(tkn[ctx.i + 1]) && tkn[ctx.i + 1] \
		!= '$' && !squote && is_previous_heredoc(ctx.i, tkn))
		{
			ctx.i++;
			process_variable(&ctx, tkn, data);
		}
		else
			ctx.res[ctx.k++] = tkn[ctx.i++];
	}
	ctx.res[ctx.k] = '\0';
	return (ctx.res);
}
