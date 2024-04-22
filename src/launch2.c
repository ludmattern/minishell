/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 17:40:25 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/22 12:15:19 by fprevot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/parse.h"
#include "../inc/exec.h"

void	free_lexed(t_token *lexed)
{
	t_token	*tmp;

	while (lexed != NULL)
	{
		tmp = lexed;
		lexed = lexed->next;
		free(tmp->value);
		tmp->value = NULL;
		free(tmp);
		tmp = NULL;
	}
}

void	lex_mallox_error(t_token *lex)
{
	free_lexed(lex);
	printf("mallox error");
	exit(EXIT_FAILURE);
}

void	free_parsing(t_node *ast, t_token *lex)
{
	free_lexed(lex);
	free_tree(ast);
	printf("mallox error");
	exit(EXIT_FAILURE);
}

void	ft_clear_memory(t_g_data *g_data)
{
	free_mini_env(g_data->mini_env);
	free(g_data->in_put);
	free(g_data->in_putsave);
}

void	update_input(t_g_data *g_data, char *pre_input)
{
	char *prompt;
	char *pre_input2;
	char *cwd = NULL;
	char *tmp = NULL;
	char *home = NULL;

	g_data->t = 0;
	tmp = getcwd(NULL, 0);
	if (tmp)
	{
		home = getenv("HOME");
		if (!home)
			home = "";
		if (ft_strnstr(tmp, home, ft_strlen(home)))
		{
			cwd = ft_strjoin("~", tmp + ft_strlen(home));
			free(tmp);
		}
		else
			cwd = tmp;
	}
	else
		cwd = ft_strdup("");
	pre_input2 = ft_strjoin(cwd, "$ ");
	free(cwd);
	if (pre_input && pre_input[0])
		prompt = ft_strjoin(pre_input, pre_input2);
	else
		prompt = ft_strdup(pre_input2);
	free(pre_input2);
	
	g_data->in_put = readline(prompt);
	free(prompt);
	free(g_data->join);
	free(g_data->path);
	if (!g_data->in_put) 
	{
		free(g_data->in_put);
		free(g_data->in_putsave);
		write(STDOUT_FILENO, "exit\n", 5);  
		ft_clear_memory(g_data);
		free(g_data);
		close_standard_fds();
		exit(EXIT_SUCCESS);
	}
}