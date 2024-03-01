/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/01 16:55:52 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

int		execute_node(t_node *node);

int	handling_command(t_node *node)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		execvp(node->expanded_args[0], node->expanded_args);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		wait(&status);
		return (0);
	}
	else
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
}

void	handling_pipeline_child(t_node *node, int pipefd[2],
		t_ast_direction direction)
{
	if (direction == AST_LEFT)
		dup2(pipefd[1], STDOUT_FILENO);
	else if (direction == AST_RIGHT)
		dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	execute_node(node);
	exit(EXIT_SUCCESS);
}

void	wait_for_children(pid_t pid1, pid_t pid2)
{
	int	status;

	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
}

void	close_pipe_fds(int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
}

int	handling_pipeline(t_node *node)
{
	int	pipefd[2];

	pid_t pid_left, pid_right;
	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid_left = fork();
	if (pid_left == 0)
		handling_pipeline_child(node->left, pipefd, AST_LEFT);
	pid_right = fork();
	if (pid_right == 0)
		handling_pipeline_child(node->right, pipefd, AST_RIGHT);
	close_pipe_fds(pipefd);
	wait_for_children(pid_left, pid_right);
	return (0);
}

/* reminder of the && operator
if the left command succeeds, execute the right command
*/
int	handling_and(t_node *node)
{
	if (execute_node(node->left) == 0)
		return (execute_node(node->right));
	return (1);
}

/* 
reminder of the OR operator
if the left command fails, execute the right command
*/
int	handling_or(t_node *node)
{
	if (execute_node(node->left) != 0)
		return (execute_node(node->right));
	return (0);
}

int	execute_node(t_node *node)
{
	if (node == NULL)
		return (0);
	if (node->type == N_CMD)
		return (handling_command(node));
	else if (node->type == N_PIPE)
		return (handling_pipeline(node));
	else if (node->type == N_AND)
		return (handling_and(node));
	else if (node->type == N_OR)
		return (handling_or(node));
	else
		return (1);
}

void	run_execution(t_data *data)
{
	execute_node(data->ast);
}
