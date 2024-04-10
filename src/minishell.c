/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/10 17:42:24 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.h"
#include "../inc/exec.h"

int	main(int argc, char **argv, char **envp)
{
	t_g_data	g_data;
	int			t;
	(void)argv;
	(void)argc;
	print_start();
	signals_init();
	g_data.exit_fail = 0;
	g_data = initialize_environnement(envp);
	while (1)
	{
		t = 0;
		update_input(&g_data);
		if (g_data.in_put[0])
		{
			if (!syntax_error(g_data.in_put, &g_data.last_exit_status))
			{
				update_data(&g_data);
				g_data.in_putsave = ft_strdup(g_data.in_put);
				free(g_data.in_put);
				g_data.in_put = NULL;
				g_data.in_put = replace_env_vars(&g_data);
				launch_lexing(&g_data);
				launch_expand(&g_data);
				launch_parsing(&g_data);
				launch_execution(&g_data);
				t = 1;
			}
			update_history(&g_data, t);
		}
	}
	ft_clear_memory(&g_data);
	return (0);
}


//LEAKS A CAUSE DE LENV au debut
//LEAKS DANS LE CAS DE > dans le lexing
//Le EOF de heredoc fonctonne pas imposible de faire entree
