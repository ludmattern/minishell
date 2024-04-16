/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/15 21:11:10 by lmattern         ###   ########.fr       */
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
						print_ast(g_data.data->ast, 0);
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
//cat CTRL+C ajouter /n
//car CTRL+\ ne doit rien faire dans le cas ou jai des child ca mrche pas
/*
cmd1 && cmd2 || cmd3
est :

    AND
    /  \
  cmd1  OR
       /  \
     cmd2 cmd3

devrait etre :

      OR
     /  \
   AND  cmd3
  /   \
cmd1 cmd2
*/

/*
possible solution

t_node	*is_op(t_token **current, t_node *root, int last_exit_status, t_g_data *g_data)
{
	t_node	*nroot;

	nroot = create_operator_node((*current), g_data);
	if (!nroot)
		return (NULL);
	last_exit_status++;
	*current = (*current)->prev;
	nroot->right = root;
	nroot->left = build_ast(current, last_exit_status, g_data);
	if (!nroot->left)
		return (NULL);
	return (nroot);
}

t_node	*build_ast(t_token **current, int last_exit_status, t_g_data *g_data)
{
	t_node	*root;

	root = NULL;
	while (*current != NULL)
	{
		if ((*current)->type == T_LPAR)
		{
			*current = (*current)->prev;
			root = build_ast(current, last_exit_status, g_data);
			if (!root)
				return (NULL);
		}
		else if ((*current)->type == T_RPAR)
			return (*current = (*current)->prev, root);
		else if ((*current)->type == T_AND || (*current)->type == \
			T_OR || (*current)->type == T_PIPE)
			return (is_op(current, root, last_exit_status, g_data));
		else if ((*current)->type == T_WORD)
		{
			if (!root)
				root = create_command_node((*current), last_exit_status, g_data);
			*current = (*current)->prev;
		}
		else
			*current = (*current)->prev;
	}
	return (root);
}

void	launch_parsing(t_g_data *g_data)
{
	g_data->data->ast = build_ast(&g_data->lexed->last, g_data->data->last_exit_status, g_data);
	// if (!data->ast)
	// 	free_parsing(data->ast, lexed);
	free_lexed(g_data->save);
}

*/