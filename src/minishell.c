/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/19 11:34:41 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.h"
#include "../inc/exec.h"

int g_heredoc_sigint = 0;

char	*init_input(void)
{
	static char buffer[1024];
	buffer[0] = 0;
	char *host = NULL;
	char *tmp;
	char *tmp2;
	char *usr;

	usr = getenv("USER");
	tmp = getenv("SESSION_MANAGER");
	if (usr && usr[0] && tmp && tmp[0])
	{
		if (ft_strlen(tmp) < 6)
			return (buffer);
		tmp2 = ft_strnstr(tmp, "local/", 6);
		if (tmp2)
		{
			if (ft_strlen(tmp2 + 6) < 6)
				return (buffer);
			host = ft_strndup(tmp2 + 6, 6);
		}
		ft_sprintf(buffer, "%s@%s:", usr, host);
		free(host);
	}
	else
		ft_sprintf(buffer, "minishell:");
	return (buffer);
}

int	main(int argc, char **argv, char **envp)
{
	t_g_data	*g_data;
	int			t;
	(void)argv;
	(void)argc;
	//print_start();
	signals_init();
	g_data = malloc(sizeof(t_g_data));
	if (!g_data)
		exit(EXIT_FAILURE);
	memset(g_data, 0, sizeof(t_g_data));
	initialize_environnement(&g_data, envp);
	g_data->pre_input = init_input();
	while (1)
	{
		t = 0;
		update_input(g_data, g_data->pre_input);
		if (g_data->in_put[0])
		{
			if (!syntax_error(g_data->in_put, &g_data->last_exit_status))
			{
				update_data(g_data);
				g_data->in_putsave = ft_strdup(g_data->in_put);
				free(g_data->in_put);
				g_data->in_put = NULL;
				g_data->in_put = replace_env_vars(g_data, 0);
				t = 1;
				if (g_data->in_put[0] != -1 || g_data->in_put[1] != 0)
				{
					launch_lexing(g_data);
					launch_expand(g_data);
					if (g_heredoc_sigint == 2)
					{
						signals_init();
						g_heredoc_sigint = 0;
					}
					else
					{
						launch_parsing(g_data);
						launch_execution(g_data);
					}
					t = 1;
				}
				else
					free(g_data->data);
			}
			update_history(g_data, t);
		}
	}
	ft_clear_memory(g_data);
	return (0);
}

// # Should skip the empty argument, and print hello after spaces
// echo - "" "  " hello
// input apres echo -n bizarre (history) //cest a cause du malloc de readline qui fait la taille du prompt et donc cela saute le reste
//cat CTRL+C ajouter /n
//car CTRL+\ ne doit rien faire dans le cas ou jai des child ca mrche pas

//segafault dans le cas d'un env -i au moment de free le mini_env