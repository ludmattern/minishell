/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/15 16:18:11 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.h"
#include "../inc/exec.h"

int g_heredoc_sigint = 0;

int	main(int argc, char **argv, char **envp)
{
	t_g_data	g_data;
	int			t;
	(void)argv;
	(void)argc;
	//print_start();
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
				if (g_data.in_put[0])
				{
					launch_lexing(&g_data);
					launch_expand(&g_data);
					if (g_heredoc_sigint == 2)
					{
						signals_init();
						g_heredoc_sigint = 0;
					}
					else
					{
						launch_parsing(&g_data);
						launch_execution(&g_data);
					}
					t = 1;
				}
				else
					g_data.last_exit_status = 0;
			}
			update_history(&g_data, t);
		}
	}
	ft_clear_memory(&g_data);
	return (0);
}

// # Should skip the empty argument, and print hello after spaces
// echo - "" "  " hello
// input apres echo -n bizarre (history) //cest a cause du malloc de readline qui fait la taille du prompt et donc cela saute le reste