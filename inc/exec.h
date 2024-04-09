/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 09:26:11 by lmattern          #+#    #+#             */
/*   Updated: 2024/04/09 11:41:08 by lmattern         ###   ########.fr       */
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
# include <signal.h>
# include <stdlib.h>
# include <linux/limits.h>
# include <string.h>
# include <errno.h>
# include <termios.h>
# include <stdbool.h>
# include <sys/stat.h>

/*
run execution
*/
int			run_execution(t_data *data);

/*
applying redirections
*/
int			apply_command_redirections(t_data *data, t_io_node *io_list);

/*
handling heredoc
*/
int			heredoc_child_process(t_data *data, int pipefd[2], const char *stop);
int			heredoc_parent_process(pid_t pid, int pipefd[2]);
int			read_heredoc_and_write_to_pipe(const char *stop, int write_fd);
bool		match_delimiter(const char *line, const char *stop);

/*
handling nodes
*/
int			handling_node(t_data *data, t_node *node, bool piped);
int			handling_pipeline(t_data *data, t_node *node);
int			handling_and(t_data *data, t_node *node, bool piped);
int			handling_or(t_data *data, t_node *node, bool piped);
int			handling_command(t_data *data, t_node *node, bool piped);

/*
handling commands utils
*/
int			wait_for_child(pid_t pid, t_data *data);
int			create_pipe(int pipefd[2]);
int			wait_for_pipeline_children(pid_t pid1, pid_t pid2);
void		command_exec_failure(t_data *data, const char *context, int exit_code)
			__attribute__((noreturn));
void		execute_command(t_data *data, t_node *node) __attribute__((noreturn));
void		restore_original_fds(t_data *data);

/*
builtins
*/
int			checking_forked_builtins(t_data *data, t_node *node);
int			ft_cd(char **args, t_env **env);
int			ft_pwd(void);
char		*ft_getenv(const char *name, char **env);
int			ft_export(char **args, t_data *data);
int			ft_env(char **args, t_env *mini_env);
int			ft_unset_vars(char **names, t_env **mini_env);
int			ft_unset(char *name, t_env **mini_env);
int			ft_exit(char **args, t_data **data);

/*
handling environnment
*/
char		**duplicate_envp(char **envp);
char		**copy_env(char **env, int size);
void		ft_sort_and_print_env(char **env, size_t size);
bool		ft_isvalid_identifier(const char *name);
int			ft_add_local(char *arg, t_env **mini_env);

/*
handling errors
*/
int			fork_creation_failure(const char *message);
int			command_redirection_failure(const char *context, int exit_code);
void		dup2_creation_failure(t_data *data, int pipefd[2]);

/*
freeing data
*/
void		free_data(t_data *data);
void		free_data_structure(t_data **data);
void		free_forked_data_structure(t_data **data);
void		close_pipe_fds(int pipefd[2]);
void		close_standard_fds(void);

/*
new
*/
void		env_err(t_env *mini_env, char *name, char *value);
void		free_mini_env(t_env *mini_env);
void		initialize_shell_variables(t_env **mini_env);
t_env		*ft_env_last(t_env *lst);
void		ft_env_add_back(t_env **lst, t_env *new);
t_env		*ft_env_new_entrie(char *name, char *value, bool is_local);
t_env		*ft_create_env_entry(const char *env_str);
t_env		*create_mini_env(char **envp);
t_g_data	initialize_environnement(char **envp);
char		*ft_get_env(char *tmp_env, t_env *mini_env);
t_env		*find_env_var(t_env *env, const char *name);
void		ft_addenv_or_update(t_env **env, const char *name, const char *value);
void		ft_removeenv(t_env **env, const char *name);
int			add_or_update_env(t_env **mini_env, char *name, char *value, bool is_local);
void		update_input(t_g_data *g_data);
void		handle_sigint(int sig);
void		handle_sigquit(int sig);
void		handle_sigint_heredoc(int sig);
void		signals_init(void);
void		ft_clear_memory(t_g_data *g_data);

#endif