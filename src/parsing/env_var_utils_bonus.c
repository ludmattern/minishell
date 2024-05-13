/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:33:26 by fprevot           #+#    #+#             */
/*   Updated: 2024/05/13 10:24:08 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/bonus/parse_bonus.h"
#include "../../inc/bonus/exec_bonus.h"

int	ft_count_wrds(char const *s)
{
	int	nb_wrd;
	int	in_wrd;

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

void	fetch_and_cleanup_env_var(t_var_context *ctx, char *tkn, t_g_data *data)
{
	ctx->tmp_env = calloc(ctx->env_length + 1, sizeof(char));
	if (!ctx->tmp_env)
		return (free(tkn), free(ctx->res), fail_exit_shell(data));
	ft_strncpy(ctx->tmp_env, tkn + ctx->start, ctx->env_length);
	ctx->env_val = ft_get_env3(ctx->tmp_env, data->mini_env, data, tkn);
	free(ctx->tmp_env);
}

void	handle_redir_and_default_value(t_var_context *ctx, char *tkn, \
		bool previous_redir, t_g_data *data)
{
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
}

void	process_variable(t_var_context *ctx, char *tkn, t_g_data *data)
{
	bool	previous_redir;

	ctx->i++;
	ctx->start = ctx->i;
	previous_redir = is_previous_redir(ctx->start, tkn);
	while (ft_isalpha(tkn[ctx->i]))
		ctx->i++;
	if ((tkn[ctx->i] >= '0' && tkn[ctx->i] <= '9') \
		|| tkn[ctx->i] == '_' || tkn[ctx->i] == '?')
		ctx->i++;
	ctx->env_length = ctx->i - ctx->start;
	if (ctx->env_length == 1 && tkn[ctx->start] == '?')
		ctx->env_val = ft_itoa(data->last_exit_status);
	else
		fetch_and_cleanup_env_var(ctx, tkn, data);
	handle_redir_and_default_value(ctx, tkn, previous_redir, data);
	quote_in_env_val(ctx->env_val);
	ctx->j = 0;
	while (ctx->env_val[ctx->j])
		ctx->res[ctx->k++] = ctx->env_val[ctx->j++];
	free(ctx->env_val);
	ctx->env_val = NULL;
}
