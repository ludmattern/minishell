/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 17:11:48 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/16 15:26:35 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

void	free_data(t_data *data)
{
	if (data == NULL)
		return ;
	free_tree(data->ast);
}

void	free_forked_data_structure(t_data **data)
{
	free_forked_data(*data);
	free(*data);
}

void	free_data_structure(t_data **data)
{
	free_data(*data);
	free(*data);
}

void	fail_exit_shell(t_g_data *g_data)
{
	ft_clear_memory(g_data);
	printf("Fatal error: failed to allocate memory.\n");
	exit(EXIT_FAILURE);
}
