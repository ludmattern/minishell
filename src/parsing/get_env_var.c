/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   get_env_var.c									  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: fprevot <fprevot@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/04/17 13:48:59 by fprevot		   #+#	#+#			 */
/*   Updated: 2024/04/18 17:25:29 by fprevot		  ###   ########.fr	   */
/*																			*/
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
			{
				free(data->path);
				free_lexed(data->lexed);
				fail_exit_shell(data);
			}
			return (env_val);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

void	init_env_ctx(t_env_context *ctx, t_g_data *data, char c, int i)
{
	if (i == 1)
	{
		ctx->squotes = false;
		ctx->dquotes = false;
		ctx->data = data;
		ctx->res = ft_strdup(data->in_putsave);
		if (!ctx->res)
			fail_exit_shell(data);
		ctx->i = 0;
		ctx->new = NULL;
		ctx->status_str = NULL;
	}
	else if (i == 2)
	{
		if (c == '"' && ctx->squotes == false)
			ctx->dquotes = !ctx->dquotes;
		else if (c == '\'' && ctx->dquotes == false)
			ctx->squotes = !ctx->squotes;
	}
}

void	process_status_var(t_env_context *ctx, int *i)
{
	ctx->status_str = ft_itoa(ctx->data->last_exit_status);
	if (!ctx->status_str)
	{
		free(ctx->res);
		fail_exit_shell(ctx->data);
	}
	ctx->new = replace_substring(ctx->res, *i, ctx->status_str, ctx->data);
	free(ctx->res);
	ctx->res = ctx->new;
	ctx->new = NULL;
	*i += ft_strlen(ctx->status_str) - 1;
	free(ctx->status_str);
}

bool	process_variable_substitution(t_env_context \
*ctx, int *i, t_g_data *data)
{
	ctx->new = get_env_var2(ctx->res, data);
	if (ctx->new == NULL)
	{
		free(ctx->res);
		ctx->res = ft_strdup("");
		return (false);
	}
	free(ctx->res);
	ctx->res = ctx->new;
	ctx->new = NULL;
	(*i)--;
	return (true);
}

char	*replace_env_vars(t_g_data *data, int i)
{
	t_env_context	ctx;

	memset(&ctx, 0, sizeof(t_env_context));
	init_env_ctx(&ctx, data, 0, 1);
	while (ctx.res && ctx.res[i] != '\0')
	{
		init_env_ctx(&ctx, data, ctx.res[i], 2);
		if (ctx.squotes == false)
		{
			if (!ctx.squotes && ctx.res[i] == '$' && ctx.res[i + 1] == '?')
				process_status_var(&ctx, &i);
			else if (!ctx.squotes && ctx.res[i] == '$'
				&& !ft_isalnum(ctx.res[i + 1])
				&& ctx.res[i + 1] != '"')
				i += 1;
			else if (!ctx.squotes && ctx.res[i] == '$'
				&& ctx.res[i + 1] != '='
				&& ctx.res[i + 1] != ':'
				&& ctx.res[i + 1] != '\0'
				&& !ft_isspace(ctx.res[i + 1]))
			{
				if (is_previous_heredoc(i, ctx.res))
				{
					if (!process_variable_substitution(&ctx, &i, data))
						return (ctx.res);
					i++;
				}
			}
		}
		i++;
	}
	return (ctx.res);
}
