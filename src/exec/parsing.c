/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:19:00 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/11 18:32:51 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

t_io_node	*create_io_node(t_io_type type, const char *value)
{
	t_io_node	*io_node = NULL;

	io_node = malloc(sizeof(t_io_node));
	io_node->type = type;
	io_node->value = ft_strdup(value);
	io_node->expanded_value = ft_strdup(value);
	io_node->here_doc = 0;
	io_node->prev = NULL;
	io_node->next = NULL;
	return (io_node);
}

t_node	*create_cmd_node(const char *command, t_io_node *io_list)
{
	t_node	*cmd_node = NULL;

	cmd_node = malloc(sizeof(t_node));
	cmd_node->type = N_CMD;
	cmd_node->args = ft_strdup(command);
	char	*args[] = {ft_strdup(command), NULL};

	cmd_node->command_path = NULL;
	cmd_node->expanded_args = malloc(2 * sizeof(char *));
	cmd_node->expanded_args[0] = args[0];
	cmd_node->expanded_args[1] = NULL;
	cmd_node->io_list = io_list;
	cmd_node->left = NULL;
	cmd_node->right = NULL;
	return (cmd_node);
}

t_node	*create_operator_node(t_node_type type, t_node *left, t_node *right)
{
	t_node	*op_node = NULL;

	op_node = malloc(sizeof(t_node));
	op_node->type = type;
	op_node->left = left;
	op_node->right = right;
	op_node->command_path = NULL;
	op_node->args = NULL;
	op_node->expanded_args = NULL;
	op_node->io_list = NULL;
	return (op_node);
}
/*
t_node	*create_ast(void)
{
	t_io_node	*cat_io = create_io_node(IO_IN, "infile.txt");
	t_node		*cat_cmd = create_cmd_node("cat", cat_io);

	cat_cmd->command_path = ft_strdup("/usr/bin/cat");
	cat_cmd->expanded_args = realloc(cat_cmd->expanded_args, 2 * sizeof(char *));
	cat_cmd->expanded_args[1] = NULL;
	t_node		*grep_cmd = create_cmd_node("grep", NULL);

	grep_cmd->command_path = ft_strdup("/usr/bin/grep");
	grep_cmd->expanded_args = realloc(grep_cmd->expanded_args, 3 * sizeof(char *));
	grep_cmd->expanded_args[1] = ft_strdup("pattern");
	grep_cmd->expanded_args[2] = NULL;
	t_node		*cat_grep_pipe = create_operator_node(N_PIPE, cat_cmd, grep_cmd);
	t_io_node	*sort_io = create_io_node(IO_OUT, "output.txt");
	t_node		*sort_cmd = create_cmd_node("wc", sort_io);

	sort_cmd->command_path = ft_strdup("/usr/bin/wc");
	sort_cmd->expanded_args = realloc(sort_cmd->expanded_args, 3 * sizeof(char *));
	sort_cmd->expanded_args[1] = ft_strdup("-l");
	sort_cmd->expanded_args[2] = NULL;
	t_node		*grep_sort_pipe = create_operator_node(N_PIPE, cat_grep_pipe, sort_cmd);
	t_node		*echo_success_cmd = create_cmd_node("/usr/bin/echo", NULL);

	echo_success_cmd->command_path = ft_strdup("/usr/bin/echo");
	echo_success_cmd->expanded_args = realloc(echo_success_cmd->expanded_args, 3 * sizeof(char *));
	echo_success_cmd->expanded_args[1] = ft_strdup("Success");
	echo_success_cmd->expanded_args[2] = NULL;
	t_node		*echo_failure_cmd = create_cmd_node("echo", NULL);

	echo_failure_cmd->command_path = ft_strdup("/usr/bin/echo");
	echo_failure_cmd->expanded_args = realloc(echo_failure_cmd->expanded_args, 4 * sizeof(char *));
	echo_failure_cmd->expanded_args[1] = ft_strdup("-n");
	echo_failure_cmd->expanded_args[2] = ft_strdup("Failure");
	echo_failure_cmd->expanded_args[3] = NULL;
	t_node		*sort_and_echo = create_operator_node(N_AND, grep_sort_pipe, echo_success_cmd);
	t_node		*root = create_operator_node(N_OR, sort_and_echo, echo_failure_cmd);

	return (root);
}
*/

/*
t_node* create_ast(void)
{
	// Building the AST for "cat << HEREDOC | grep "pattern" | wc -l > output.txt && echo "Success" || echo -n "Failure"
	// Construct the "cat < input.txt" command
	//t_io_node* cat_io = create_io_node(IO_IN, "input.txt");
	t_io_node* echo_io = create_io_node(IO_OUT, "outfile.txt");
	t_node* echo_cmd = create_cmd_node("echo", echo_io);
	echo_cmd->command_path = strdup("/usr/bin/echo");
	echo_cmd->expanded_args = realloc(echo_cmd->expanded_args, 3 * sizeof(char*));
	echo_cmd->expanded_args[1] = strdup('$?');
	echo_cmd->expanded_args[2] = NULL;

	return echo_cmd;
}
*/


// t_node *create_ast(void) {
//     // Créer la commande "echo bonjour"
//     t_node *echo_bonjour_cmd = create_cmd_node("echo", NULL); // Pas de redirection d'IO ici
//     echo_bonjour_cmd->command_path = ft_strdup("/usr/bin/echo"); // Assurez-vous que le chemin est correct pour votre système
//     echo_bonjour_cmd->expanded_args = realloc(echo_bonjour_cmd->expanded_args, 3 * sizeof(char *));
//     echo_bonjour_cmd->expanded_args[1] = ft_strdup("-n");
//     echo_bonjour_cmd->expanded_args[2] = NULL;

//     // Créer la commande "echo au-revoir"
//     t_node *echo_au_revoir_cmd = create_cmd_node("echo", NULL); // Pas de redirection d'IO ici
//     echo_au_revoir_cmd->command_path = ft_strdup("echo"); // Assurez-vous que le chemin est correct pour votre système
//     echo_au_revoir_cmd->expanded_args = realloc(echo_au_revoir_cmd->expanded_args, 4 * sizeof(char *));
//     echo_au_revoir_cmd->expanded_args[1] = ft_strdup("-n");
//     echo_au_revoir_cmd->expanded_args[2] = ft_strdup("au-revoir");
//     echo_au_revoir_cmd->expanded_args[3] = NULL;

//     // Utiliser l'opérateur AND pour connecter les deux commandes "echo"
//     t_node *root = create_operator_node(N_AND, echo_bonjour_cmd, echo_au_revoir_cmd);

//     return root;
// }

t_node *create_ast(void)
{
	t_node *cd = create_cmd_node("pwd", NULL);
	cd->command_path = ft_strdup("pwd");
	cd->expanded_args = realloc(cd->expanded_args, 2 * sizeof(char *));
	cd->expanded_args[1] = NULL;
	return (cd);
}

char	**duplicate_envp(char **envp)
{
	int		i;
	int		j;
	int		k;
	char	**minishell_env;

	i = 0;
	j = 0;
	k = 0;
	while (envp && envp[i] != NULL)
		i++;
	minishell_env = malloc((i + 1) * sizeof(char *));
	if (minishell_env == NULL) {
		perror("Failed to allocate memory for minishell_env");
		exit(EXIT_FAILURE);
	}
	while (j < i)
	{
		minishell_env[j] = malloc(strlen(envp[j]) + 1);
		if (minishell_env[j] == NULL)
		{
			perror("Failed to allocate memory for minishell_env[j]");
			while (k < j)
				free(minishell_env[k++]);
			free(minishell_env);
			exit(EXIT_FAILURE);
		}
		ft_strlcpy(minishell_env[j], envp[j], ft_strlen(envp[j]) + 1);
		j++;
	}
	minishell_env[i] = NULL;

	return (minishell_env);
}

void	parsing(t_data **data, int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	*data = malloc(sizeof(t_data));
	if (*data == NULL)
		exit (EXIT_GENERAL_ERROR);
	ft_bzero(*data, sizeof(t_data));
	(*data)->env = duplicate_envp(envp);
	(*data)->ast = create_ast();
	(*data)->last_exit_status = EXIT_SUCCESS;
}
