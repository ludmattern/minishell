/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 09:26:11 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/02 12:59:26 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "../libft/inc/libft.h"
# include "../libft/inc/ft_printf.h"
# include "../libft/inc/get_next_line.h"
# include "./error_codes.h"
# include "./minishell.h"
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <linux/limits.h>
# include <string.h>
# include <errno.h>
# include <termios.h>
# include <stdbool.h>
# include <sys/stat.h>

typedef enum e_ast_direction
{
	AST_LEFT,
	AST_RIGHT
}	t_ast_direction;

/*
debug only
*/
t_node	*create_ast(void);
void	print_ast(t_node *node, int depth);
void	parsing(t_data **data, int argc, char **argv, char **envp);

/*
run execution
*/
int		run_execution(t_data *data);

/*
applying redirections
*/
int		apply_command_redirections(t_data *data, t_io_node *io_list);

/*
handling heredoc
*/
int		heredoc_child_process(t_data *data, int pipefd[2], const char *stop);
int		heredoc_parent_process(pid_t pid, int pipefd[2]);
int		read_heredoc_and_write_to_pipe(const char *stop, int write_fd);
bool	match_delimiter(const char *line, const char *stop);

/*
handling nodes
*/
int		handling_node(t_data *data, t_node *node, bool piped);
int		handling_pipeline(t_data *data, t_node *node);
int		handling_and(t_data *data, t_node *node, bool piped);
int		handling_or(t_data *data, t_node *node, bool piped);
int		handling_command(t_data *data, t_node *node, bool piped);

/*
handling commands utils
*/
int		wait_for_child(pid_t pid, t_data *data);
int		create_pipe(int pipefd[2]);
int		wait_for_pipeline_children(pid_t pid1, pid_t pid2);

/*
builtins
*/
int		checking_forked_builtins(t_data *data, t_node *node);
int		ft_cd(char **args, char **env);
int		ft_pwd(void);
char	*ft_getenv(const char *name, char **env);
int		ft_export(char **args, char ***env);
void	ft_print_env_sorted(char **env);
int		ft_env(char **args, char ***env);
int		ft_unset_vars(char **names, char ***env);
void	print_env_var(const char *var);
int		ft_unset(char *name, char ***env);

/*
handling environnment
*/
char	**ft_addenv(char *name, char *value, char ***env);
char	**duplicate_envp(char **envp);
char	**copy_env(char **env, int size);
void	sort_env(char **env, int size);
void	print_and_free_env(char **env);
size_t	count_env_entries(char **envp);

/*
handling errors
*/
int		fork_creation_failure(const char *message);
int		command_redirection_failure(const char *context, int exit_code);
void	dup2_creation_failure(t_data *data, int pipefd[2]);

/*
freeing data
*/
void	free_data(t_data *data);
void	free_data_structure(t_data **data);
void	free_forked_data_structure(t_data **data);
void	close_pipe_fds(int pipefd[2]);
void	close_standard_fds(void);

#endif