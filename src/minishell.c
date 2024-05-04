/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/05/04 11:01:35 by fprevot          ###   ########.fr       */
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
			if (!host)
				return (NULL);
		}
		return (ft_sprintf(buffer, "%s@%s:", usr, host), free(host), buffer);
	}
	else
		return (ft_sprintf(buffer, "minishell:"), buffer);
}

void	handle_user_input(t_g_data *g_data)
{
	update_data(g_data);
	if (g_data->in_put[0] != -1 || g_data->in_put[1] != 0)
	{
		launch_lexing(g_data);
		if (g_data->lexed->error == -1)
		{
			free_lexed(g_data->lexed);
			fail_exit_shell(g_data);
		}
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

void	main_clean_exit(t_g_data *g_data)
{
	free_mini_env(g_data->mini_env);
	free(g_data);
	close_standard_fds();
	exit(EXIT_GENERAL_ERROR);
}

void	init_minishell(t_g_data **g_data, char **env, char **av, int ac)
{
	(void)av;
	if (ac != 1)
	{
		ft_putstr_fd("minishell: no arguments needed\n", STDERR_FILENO);
		close_standard_fds();
		exit(EXIT_GENERAL_ERROR);
	}
	signals_init();
	*g_data = malloc(sizeof(t_g_data));
	if (!*g_data)
	{
		close_standard_fds();
		exit(EXIT_GENERAL_ERROR);
	}
	memset(*g_data, 0, sizeof(t_g_data));
	(*g_data)->mini_env = create_mini_env(env, g_data);
	if (!initialize_shell_variables(&(*g_data)->mini_env))
		main_clean_exit(*g_data);
	(*g_data)->pre_input = init_bash();
	if (!(*g_data)->pre_input)
		main_clean_exit(*g_data);
	(*g_data)->t = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_g_data	*g_data;

	init_minishell(&g_data, envp, argv, argc);
	while (1)
	{
		update_input(g_data, g_data->pre_input);
		if (g_data->in_put[0])
		{
			if (!syntax_error(g_data->in_put, &g_data->last_exit_status))
				handle_user_input(g_data);
			update_history(g_data);
		}
	}
	ft_clear_memory(g_data);
	return (0);
}
/*
Au moment de lexpand j parcours env val si jai des quote deans je les transform en -2 -3 et au moment de skip les quote le les repasse en quote
*/

//MAJ le code d'erreur en cas de ctrl+C (a verifier apres dans les ss processus aussi) (a voir pour les autres signaux)
/*heredoc :
		si le delimiteur est entre guillemets ("EOF" ou 'EOF') ne pas expand les var d'environnement dans le resultat
		sinon, le faire (cas actuel)*/ //faire fonction is_prev_heredoc.
/*
cat :
		//si ctrl+c dans cat alors on sort de toute la ligne de commande 
		//cat CTRL+C ajouter /n
*/


/*pb de free dans command_path*/


/*Malloc fail cd sur gt_get_env a check*/