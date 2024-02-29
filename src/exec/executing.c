/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/02/29 10:48:08 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

int	execute_node(t_node *node);

int	handling_command(t_node *node)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		printf("Child executing command: %s\n", node->args);
		exit(0); // Simulate success of execution
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		printf("Parent waiting for child executing command: %s\n", node->args);
		wait(&status);
		return (0); // Simulate command run_execution success
	}
	else
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
}

int	handling_pipeline_child(t_node *node)
{
	if (node == NULL)
		return (0);
	return (execute_node(node));
}

int	handling_pipeline(t_node *node)
{
	int	status_left;
	int	status_right;

	printf("Setting up a pipe\n");
	status_left = handling_pipeline_child(node->left);
	if (status_left != 0)
		printf("left branch of pipe failed");
	status_right = handling_pipeline_child(node->right);
	if (status_right != 0)
		printf("right branch of pipe failed");
	printf("Finished handling pipe\n");
	return (status_right);
}


/* reminder of the && operator
* if the left command succeeds, execute the right command
*/
int	handling_and(t_node *node)
{
	printf("Executing AND node\n");
	if (execute_node(node->left) == 0)
	{
		printf("success of AND left branch, executing right branch \n");
		return (execute_node(node->right));
	}
	printf("failure of AND left branch, skipping right branch\n");
	return (1); // Simulate failure if left command fails
}

/* reminder of the OR operator
* if the left command fails, execute the right command
*/
int	handling_or(t_node *node)
{
	printf("Executing OR node\n");
	if (execute_node(node->left) != 0)
	{
		printf("failure of OR left branch, executing right branch\n");
		return (execute_node(node->right));
	}
	printf("success of OR left branch\n");
	return (0);
}

int execute_node(t_node *node)
{
	if (node == NULL)
		return 0;
	if (node->type == N_CMD)
		return handling_command(node);
	else if (node->type == N_PIPE)
		return handling_pipeline(node);
	else if (node->type == N_AND)
		return handling_and(node);
	else if (node->type == N_OR)
		return handling_or(node);
	else
		printf("Unknown node type encountered\n");
	return 1;
}

// Main execution entry point
void run_execution(t_data* data)
{
	execute_node(data->ast);
}


