/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 10:19:00 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/06 17:53:56 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

// Helper functions to create different nodes
t_io_node* create_io_node(t_io_type type, const char* value)
{
	t_io_node* io_node = malloc(sizeof(t_io_node));
	io_node->type = type;
	io_node->value = strdup(value);
	io_node->expanded_value = strdup(value);
	io_node->here_doc = 0;
	io_node->prev = NULL;
	io_node->next = NULL;
	return io_node;
}

t_node* create_cmd_node(const char* command, t_io_node* io_list)
{
	t_node* cmd_node = malloc(sizeof(t_node));
	cmd_node->type = N_CMD;
	cmd_node->args = strdup(command); // For simplicity, args are duplicated here
	char* args[] = { strdup(command), NULL }; // Assuming args are simple for this example
	cmd_node->expanded_args = malloc(2 * sizeof(char*));
	cmd_node->expanded_args[0] = args[0];
	cmd_node->expanded_args[1] = NULL;
	cmd_node->io_list = io_list;
	cmd_node->left = NULL;
	cmd_node->right = NULL;
	return cmd_node;
}

t_node* create_operator_node(t_node_type type, t_node* left, t_node* right)
{
	t_node* op_node = malloc(sizeof(t_node));
	op_node->type = type;
	op_node->left = left;
	op_node->right = right;
	op_node->args = NULL;
	op_node->expanded_args = NULL;
	op_node->io_list = NULL;
	return op_node;
}

t_node* create_ast(void)
{
	// Building the AST for "cat << HEREDOC | grep "pattern" | wc -l > output.txt && echo "Success" || echo -n "Failure"
	// Construct the "cat < input.txt" command
	//t_io_node* cat_io = create_io_node(IO_IN, "input.txt");
	t_io_node* cat_io = create_io_node(IO_HEREDOC, "EOF");
	t_node* cat_cmd = create_cmd_node("/usr/bin/cot", cat_io);
	cat_cmd->command_path = strdup("/usr/bin/cot");
	cat_cmd->expanded_args = realloc(cat_cmd->expanded_args, 2 * sizeof(char*));
	cat_cmd->expanded_args[1] = NULL;

	// Construct the "grep "pattern"" command
	t_node* grep_cmd = create_cmd_node("grep", NULL);
	grep_cmd->command_path = strdup("/usr/bin/grep");
	grep_cmd->expanded_args = realloc(grep_cmd->expanded_args, 3 * sizeof(char*));
	grep_cmd->expanded_args[1] = strdup("pattern");
	grep_cmd->expanded_args[2] = NULL;

	// Pipe "cat" and "grep"
	t_node* cat_grep_pipe = create_operator_node(N_PIPE, cat_cmd, grep_cmd);

	// Construct the "sort > output.txt" command
	t_io_node* sort_io = create_io_node(IO_OUT, "output.txt");
	t_node* sort_cmd = create_cmd_node("wc", sort_io);
	sort_cmd->command_path = strdup("/usr/bin/wc");
	sort_cmd->expanded_args = realloc(sort_cmd->expanded_args, 3 * sizeof(char*));
	sort_cmd->expanded_args[1] = strdup("-l");
	sort_cmd->expanded_args[2] = NULL;

	// Pipe "grep" and "sort"
	t_node* grep_sort_pipe = create_operator_node(N_PIPE, cat_grep_pipe, sort_cmd);

	// Construct the "echo "Success"" command
	t_node* echo_success_cmd = create_cmd_node("cot", NULL);
	echo_success_cmd->command_path = strdup("/usr/bin/cot");
	echo_success_cmd->expanded_args = realloc(echo_success_cmd->expanded_args, 3 * sizeof(char*));
	echo_success_cmd->expanded_args[1] = strdup("Success");
	echo_success_cmd->expanded_args[2] = NULL;

	// Construct the "echo -n "Failure"" command
	t_node* echo_failure_cmd = create_cmd_node("/usr/bin/", NULL);
	echo_failure_cmd->command_path = strdup("/usr/bin/");
	echo_failure_cmd->expanded_args = realloc(echo_failure_cmd->expanded_args, 4 * sizeof(char*));
	echo_failure_cmd->expanded_args[1] = strdup("-n");
	echo_failure_cmd->expanded_args[2] = strdup("Failure");
	echo_failure_cmd->expanded_args[3] = NULL;

	// AND operator between "sort > output.txt" and "echo "Success""
	t_node* sort_and_echo = create_operator_node(N_AND, grep_sort_pipe, echo_success_cmd);

	// OR operator for the whole command
	t_node* root = create_operator_node(N_OR, sort_and_echo, echo_failure_cmd);

	return root;
}