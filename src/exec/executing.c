/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 14:00:32 by lmattern          #+#    #+#             */
/*   Updated: 2024/02/28 17:42:19 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/exec.h"

int execute_command(t_node *node);

int execute_pipe(t_node *node) {
    int status_left = 0, status_right = 0;

    if (node->left) status_left = execute_command(node->left);
    if (node->right) status_right = execute_command(node->right);

    // Assuming a success if both commands execute successfully
    // In a real scenario, we'd also need to actually pipe output from left to input of right
    printf("Finished handling pipe\n");
    return (status_left == 0 && status_right == 0) ? 0 : 1;
}

int execute_command(t_node *node) {
    if (node == NULL) return 0;

    if (node->type == N_CMD) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("Child executing command: %s\n", node->args);
            exit(0); // Simulate success
        } else if (pid > 0) {
            int status;
            printf("Parent waiting for child executing command: %s\n", node->args);
            wait(&status);
            return 0; // Simulate command execution success
        } else {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    } else if (node->type == N_PIPE) {
        printf("Setting up a pipe\n");
        return execute_pipe(node);
    } else if (node->type == N_AND) {
        printf("Executing AND node\n");
        if (execute_command(node->left) == 0) {
            return execute_command(node->right);
        }
        return 1; // Simulate failure if left command fails
    } else if (node->type == N_OR) {
        printf("Executing OR node\n");
        if (execute_command(node->left) != 0) {
            return execute_command(node->right);
        }
        return 0; // Simulate success if left command succeeds
    } else {
        printf("Unknown node type encountered\n");
    }
    return 1; // General failure for unknown types
}



// Main execution entry point
void execute_ast(t_data* data) {
    execute_command(data->ast);
}
