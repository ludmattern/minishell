/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmattern <lmattern@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 09:26:11 by lmattern          #+#    #+#             */
/*   Updated: 2024/03/11 18:25:21 by lmattern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "../libft/inc/libft.h"
# include "../libft/inc/ft_printf.h"
# include "../libft/inc/get_next_line.h"
# include "./error_codes.h"
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include <stdlib.h>
#include <linux/limits.h>
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

typedef enum e_node_type
{
	N_PIPE,
	N_AND,
	N_OR,
	N_CMD
}	t_node_type;

typedef enum e_io_type
{
	IO_IN,
	IO_OUT,
	IO_HEREDOC,
	IO_APPEND
}	t_io_type;

typedef struct s_io_node
{
	t_io_type			type;
	char				*value;
	char				*expanded_value;
	int					here_doc;
	struct s_io_node	*prev;
	struct s_io_node	*next;
}	t_io_node;

typedef struct s_node
{
	t_node_type			type;
	t_io_node			*io_list;
	char				*args;
	char				*command_path;
	char				**expanded_args;
	struct s_node		*left;
	struct s_node		*right;
}	t_node;

typedef struct s_data
{
	t_node			*ast;
	int				last_exit_status;
	int				stdin;
	int				stdout;
	char			**env;
	bool			signint_child;
	bool			heredoc_sigint;
	struct termios	initial_terminal_attributes;
}	t_data;

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
int		handling_node(t_data *data, t_node *node);
int		handling_pipeline(t_data *data, t_node *node);
int		handling_and(t_data *data, t_node *node);
int		handling_or(t_data *data, t_node *node);
int		handling_command(t_data *data, t_node *node);

/*
handling commands utils
*/
int		wait_for_child(pid_t pid, t_data *data);
int		create_pipe(int pipefd[2]);
void	wait_for_pipeline_children(pid_t pid1, pid_t pid2);

/*
builtins
*/
int		checking_builtins(t_data *data, t_node *node);
int		ft_cd(char **args, char **env);
int		ft_pwd(char **args);
char	*ft_getenv(const char *name, char **env);

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
void	close_pipe_fds(int pipefd[2]);
void	close_standard_fds(void);

#endif