/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/02/29 16:05:02 by lmattern         ###   ########.fr       */
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
		//printf("Child executing command: %s\n", node->args);
		execvp(node->expanded_args[0], node->expanded_args);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		//printf("Parent waiting for child executing command: %s\n", node->args);
		wait(&status);
		return (0);
	}
	else
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
}

int	handling_pipeline(t_node *node)
{
    int pipefd[2];
    pid_t pid_left, pid_right;

    //printf("Setting up a pipe\n");
    if (pipe(pipefd) < 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if ((pid_left = fork()) == 0) {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        execute_node(node->left);
        exit(EXIT_SUCCESS);
    }
    if ((pid_right = fork()) == 0) {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        close(pipefd[0]);

        execute_node(node->right);
        exit(EXIT_SUCCESS);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(pid_left, NULL, 0);
    waitpid(pid_right, NULL, 0);
    //printf("Finished handling pipe\n");

    return 0;
}



/*
* reminder of the && operator
* if the left command succeeds, execute the right command
*/
int	handling_and(t_node *node)
{
	//printf("Executing AND node\n");
	if (execute_node(node->left) == 0)
	{
		//printf("success of AND left branch, executing right branch \n");
		return (execute_node(node->right));
	}
	//printf("failure of AND left branch, skipping right branch\n");
	return (1);
}

/* 
* reminder of the OR operator
* if the left command fails, execute the right command
*/
int	handling_or(t_node *node)
{
	//printf("Executing OR node\n");
	if (execute_node(node->left) != 0)
	{
		//printf("failure of OR left branch, executing right branch\n");
		return (execute_node(node->right));
	}
	//printf("success of OR left branch\n");
	return (0);
}

int	execute_node(t_node *node)
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
		//printf("Unknown node type encountered\n");
	return 1;
}

void	run_execution(t_data* data)
{
	execute_node(data->ast);
}
