/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/17 10:57:39 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

/*
Handles the child process of a pipeline.
*/
void	handling_pipeline_child(t_data *data, t_node *node, int pipefd[2],
		t_ast_direction direction)
{
	int	status;

	status = EXIT_SUCCESS;
	if (direction == AST_LEFT)
		status = dup2(pipefd[1], STDOUT_FILENO);
	else if (direction == AST_RIGHT)
		status = dup2(pipefd[0], STDIN_FILENO);
	if (status < 0)
		dup2_creation_failure(data, pipefd);
	close_pipe_fds(pipefd);
	handling_node(data, node, true);
	//free_data_structure(&data);
	close_standard_fds();
	exit(EXIT_SUCCESS);
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

	if (create_pipe(pipefd) != EXIT_SUCCESS)
		return (EXIT_PIPE_FAILURE);
	pid_left = fork();
	if (pid_left < 0)
		return (close_pipe_fds(pipefd), fork_creation_failure("fork"));
	if (pid_left == 0)
		handling_pipeline_child(data, node->left, pipefd, AST_LEFT);
	pid_right = fork();
	if (pid_right < 0)
		return (close_pipe_fds(pipefd), fork_creation_failure("fork"));
	if (pid_right == 0)
		handling_pipeline_child(data, node->right, pipefd, AST_RIGHT);
	close_pipe_fds(pipefd);
	wait_for_pipeline_children(pid_left, pid_right);
	return (EXIT_SUCCESS);
}
