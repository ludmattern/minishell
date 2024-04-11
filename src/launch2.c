/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fprevot <fprevot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 17:40:25 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/11 16:44:15 by fprevot          ###   ########.fr       */
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
}

void	update_input(t_g_data *g_data)
{/*
	char *prompt;
	char *prompt2;
	const char *green = "\033[1;36m";
	const char *blue = "\033[1;35m";
	const char *reset = "\033[0m"; 
	g_data->path = getcwd(NULL, 0);
	char *colored_path = ft_strjoin(green, g_data->path);
	char *colored_prompt = ft_strjoin(colored_path, blue);
	free(colored_path);
	prompt = ft_strjoin(colored_prompt, " 🤖 $> ");
	free(colored_prompt);
	prompt = ft_strjoin(prompt, reset); 
*/
	char *prompt;
	prompt = " $> ";//version sans prompt pimpe pour le tester
	
	g_data->in_put = readline(prompt);
	free(g_data->join);
	free(g_data->path);
	if (!g_data->in_put) 
	{
		write(STDOUT_FILENO, "exit\n", 5);  
		ft_clear_memory(g_data);
		exit(EXIT_SUCCESS);
	}
}