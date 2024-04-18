/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 17:40:25 by fprevot           #+#    #+#             */
/*   Updated: 2024/04/18 21:32:05 by lmattern         ###   ########.fr       */
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
{/*
	char *prompt;
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
	char *pre_input2;
	char *cwd = NULL;
	char *tmp = NULL;
	/*g_data->path = getcwd(NULL, 0);
	char *user = getenv("USER");
	char *z = extract_hostname_from_env();
	prompt = ft_strjoin(user, "@");
	prompt = ft_strjoin(prompt, z);
	prompt = ft_strjoin(prompt, g_data->path);
	prompt = ft_strjoin(prompt, "$");*/
	tmp = getcwd(NULL, 0);
	if (tmp)
	{
		if (ft_strnstr(tmp, getenv("HOME"), ft_strlen(getenv("HOME"))))
		{
			cwd = ft_strjoin("~", tmp + ft_strlen(getenv("HOME")));
			free(tmp);
		}
		else
			cwd = tmp;
	}
	else
		cwd = ft_strdup("");
	pre_input2 = ft_strjoin(cwd, "$ ");
	free(cwd);
	prompt = ft_strjoin(pre_input, pre_input2);
	free(pre_input2);
	
	g_data->in_put = readline(prompt);
	free(prompt);
	//free(user);
	//free(z);
	free(g_data->join);
	free(g_data->path);
	if (!g_data->in_put) 
	{
		free(g_data->in_put);
		free(g_data->in_putsave);
		write(STDOUT_FILENO, "exit\n", 5);  
		ft_clear_memory(g_data);
		close_standard_fds();
		exit(EXIT_SUCCESS);
	}
}