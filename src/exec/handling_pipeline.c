/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/05 13:15:56 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

int		handling_node(t_data *data, t_node *node);

/*
Handles the child process of a pipeline.
*/
void	handling_pipeline_child(t_data *data, t_node *node, int pipefd[2],
		t_ast_direction direction)
{
	if (direction == AST_LEFT)
		dup2(pipefd[1], STDOUT_FILENO);
	else if (direction == AST_RIGHT)
		dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	handling_node(data, node);
	exit(EXIT_SUCCESS);
}

/*
Waits for the child processes to finish.
*/
void	wait_for_pipeline_children(pid_t pid1, pid_t pid2)
{
	int	status;

	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
}

/*
Closes the pipe file descriptors.
*/
void	close_pipe_fds(int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
}

/*
Handles the pipeline by creating two child processes and connecting them with a 
pipe.
*/
int	handling_pipeline(t_data *data, t_node *node)
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;

	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid_left = fork();
	if (pid_left == 0)
		handling_pipeline_child(data, node->left, pipefd, AST_LEFT);
	pid_right = fork();
	if (pid_right == 0)
		handling_pipeline_child(data, node->right, pipefd, AST_RIGHT);
	close_pipe_fds(pipefd);
	wait_for_pipeline_children(pid_left, pid_right);
	return (0);
}
