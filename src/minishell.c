/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/21 16:15:06 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.h"
#include "../inc/exec.h"

int	g_heredoc_sigint = 0;

char	*init_bash(void)
{
	static char	buffer[1024];
	char		*host;
	char		*tmp;
	char		*tmp2;
	char		*usr;

	host = NULL;
	buffer[0] = 0;
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
		return (ft_sprintf(buffer, "%s@%s:", usr, host), free(host), buffer);
	}
	else
		return (ft_sprintf(buffer, "minishell:"), buffer);
}

int	main(int argc, char **argv, char **envp)
{
	t_g_data	*g_data;

	(void)argv;
	(void)argc;
	initialize_environnement(&g_data, envp);
	while (1)
	{
		g_data->t = 0;
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
				g_data->t = 1;
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
				}
				else
					free(g_data->data);
			}
			update_history(g_data);
		}
	}
	ft_clear_memory(g_data);
	return (0);
}

//cat CTRL+C ajouter /n
//leak en cas de CTRL+D (free le pointeur general de la structure)