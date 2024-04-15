/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 17:40:15 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/15 18:23:09 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.h"
#include "../inc/exec.h"

void	launch_lexing(t_g_data *g_data)
{
	g_data->lexed = lex_me(g_data->in_put);
	if (g_data->exit_fail == -1)
		lex_mallox_error(g_data->lexed);
	g_data->save = g_data->lexed;
	g_data->lexed->g_data = g_data;
}

void	launch_parsing(t_g_data *g_data)
{
	g_data->data->ast = build_ast(&g_data->lexed->first, g_data->data->last_exit_status, g_data);
	// if (!data->ast)
	// 	free_parsing(data->ast, lexed);
	free_lexed(g_data->save);
}

void	launch_execution(t_g_data *g_data)
{
	g_data->data->g_data = g_data;
	g_data->last_exit_status = run_execution(g_data->data);
	g_data->mini_env = g_data->data->mini_env;
	free_data_structure(&g_data->data);
}

void	update_history(t_g_data *g_data, int t)
{
	if (t == 1)
	{
		add_history(g_data->in_putsave);
		free(g_data->in_putsave);
		g_data->in_putsave = NULL;
		free(g_data->in_put);
		g_data->in_put = NULL;
	}
	else
	{
		add_history(g_data->in_put);
		free(g_data->in_put);
		free(g_data->in_putsave);
		g_data->in_putsave = NULL;
		free(g_data->in_putsave);
		g_data->in_put = NULL;
	}
	
}

void	update_data(t_g_data *g_data)
{
	g_data->data = malloc(sizeof(t_data));
	//if (!data)
	//	return (printf("MALLOC ERROR"));
	ft_bzero(g_data->data, sizeof(t_data));
	g_data->data->mini_env = g_data->mini_env;
	g_data->data->last_exit_status = g_data->last_exit_status;
}